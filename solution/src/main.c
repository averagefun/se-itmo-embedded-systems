#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io_utils.h"
#include "pe_file.h"

#define APP_NAME "section-extractor"

void usage(FILE* f) {
    fprintf(f, "Usage: " APP_NAME " <in_file> <section_name> <out_file>\n");
}

bool print_read_status(enum read_status read_status) {
    if (read_status == READ_OK) {
        fprintf(stdout, "%s", read_status_msgs_en[READ_OK]);
        return true;
    } else {
        fprintf(stderr, "%s", read_status_msgs_en[read_status]);
        return false;
    }
}

bool print_write_status(enum write_status write_status) {
    if (write_status == WRITE_OK) {
        fprintf(stdout, "%s", write_status_msgs_en[WRITE_OK]);
        return true;
    } else {
        fprintf(stderr, "%s", write_status_msgs_en[write_status]);
        return false;
    }
}

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    if (argc != 4) {
        usage(stdout);
        return 1;
    }

    struct PEFile pe_file;
    enum read_status read_status = read_file(argv[1], read_pe_file, &pe_file);
    if (!print_read_status(read_status)) return read_status;

    struct PESection pe_section = {.name = argv[2], .pe_file = &pe_file};
    read_status = read_file(argv[1], read_pe_section, &pe_section);
    if (!print_read_status(read_status)) return read_status;

    enum write_status write_status =
        write_file(argv[3], write_pe_section, &pe_section);
    if (!print_write_status(write_status)) return write_status;

    pe_file_destroy(&pe_file);
    pe_section_destroy(&pe_section);
}
