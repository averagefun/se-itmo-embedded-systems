#include "calc.h"
#include "builder.h"
#include "ast.h"
#include "stdbool.h"

#include "string.h"

static int64_t calc_expr(char *input) {
    return calc_ast(build_ast(input));
}


#define IMPLEMENT_ADD_ACTION(action, ch) \
    static void calc_Action_Add_##action(struct calculator_i *calc) { \
        strcat(calc->buf, ch); \
        calc->cur += strlen(ch); \
} \


IMPLEMENT_ADD_ACTION(0, "0")
IMPLEMENT_ADD_ACTION(1, "1")
IMPLEMENT_ADD_ACTION(2, "2")
IMPLEMENT_ADD_ACTION(3, "3")
IMPLEMENT_ADD_ACTION(4, "4")
IMPLEMENT_ADD_ACTION(5, "5")
IMPLEMENT_ADD_ACTION(6, "6")
IMPLEMENT_ADD_ACTION(7, "7")
IMPLEMENT_ADD_ACTION(8, "8")
IMPLEMENT_ADD_ACTION(9, "9")

IMPLEMENT_ADD_ACTION(Plus, "+")
IMPLEMENT_ADD_ACTION(Sub, "-")
IMPLEMENT_ADD_ACTION(Mul, "*")
IMPLEMENT_ADD_ACTION(Div, "/")

IMPLEMENT_ADD_ACTION(Fact, "!")

IMPLEMENT_ADD_ACTION(Open_Bracket, "(")
IMPLEMENT_ADD_ACTION(Close_Bracket, ")")

#undef IMPLEMENT_ADD_ACTION

static void calc_Action_Change_Layout(struct calculator_i *calc) {
    calc->layout = (calc->layout + 1) % LAYOUTS_N;
}

static void int64_to_string(int64_t num, char *str) {
    int64_t n = num;
    bool is_negative = false;
    int i = 0;

    if (n < 0) {
        is_negative = true;
        n = -n;
    }

    do {
        str[i++] = '0' + (n % 10);
        n /= 10;
    } while (n > 0);

    if (is_negative) {
        str[i++] = '-';
    }

    for (int j = 0; j < i / 2; j++) {
        char tmp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = tmp;
    }

    str[i] = '\0';
}

static void calc_Apply_Equation(struct calculator_i *calc) {
	int64_t res = calc_expr(calc->buf);
	int64_to_string(res, calc->buf);
    calc->cur = strlen(calc->buf);
    calc->last = calc->cur;
}

static void calc_Action_CE(struct calculator_i *calc) {
    for (size_t i = calc->last; i < sizeof(calc->buf); i++) {
        calc->buf[i] = '\0';
    }
    calc->cur = calc->last;
}

static void calc_Action_C(struct calculator_i *calc) {
    for (size_t i = 0; i < sizeof(calc->buf); i++) {
        calc->buf[i] = '\0';
    }
    calc->last = 0;
    calc->cur = 0;
}

static void calc_Action_Backspace(struct calculator_i *calc) {
	calc->buf[calc->cur-1] = '\0';
	calc->cur -= 1;
}

static calc_Action *actions[LAYOUTS_N][LAYOUT_WIDTH * LAYOUT_HEIGHT] = {
        {
                calc_Action_Add_1, calc_Action_Add_2,   calc_Action_Add_3,
                calc_Action_Add_4, calc_Action_Add_5,   calc_Action_Add_6,
                calc_Action_Add_7, calc_Action_Add_8,   calc_Action_Add_9,
                calc_Action_Add_0, calc_Apply_Equation, calc_Action_Change_Layout
        },
        {
                calc_Action_Add_Plus, calc_Action_Add_Sub, calc_Action_Add_Open_Bracket,
                calc_Action_Add_Mul,  calc_Action_Add_Div, calc_Action_Add_Close_Bracket,
                calc_Action_Add_Fact, NULL,                calc_Action_Backspace,
                calc_Action_CE,       calc_Action_C,       calc_Action_Change_Layout
        }
};

void calc_Call_Action(struct calculator_i *calc, uint8_t index) {
    calc_Action *action = actions[calc->layout][index];
    if(action == NULL) {
        return;
    }
    action(calc);
    if(calc->layout != LAYOUT_DEFAULT && action != calc_Action_Change_Layout) {
        calc->layout = LAYOUT_DEFAULT;
    }
}
