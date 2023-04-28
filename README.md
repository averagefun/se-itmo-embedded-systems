# PE file reader
---
Чтение PE файла

# PE файл

PE файлы используются в ОС Windows для хранения объектных и исполняемых файлов. 
Для "совместимости" со старыми операционными системами PE файл начинается с корректной программы для MS-DOS, которая выводит сообщение "This program cannot be run in DOS mode." Поэтому заголовок и данные в PE файле смещены от начала файла и эффективное смещение записано со смещением `0x3c` от начала файла.

По данному смещению записано magic value `PE\0\0', за которым следует заголовок и данные.

Для чтения заголовков удобно использовать упакованные структуры.  

## Упаковка данных

Для упаковки структур используем `#pragma packed(...)` для компилятора Visual C и `__attribute__((packed))` для компиляторов gcc и clang. Используя директивы препорцессора можно выяснить вид компилятора и написать портабельный код:

```c
#ifdef __clang__
    printf("Hello world from clang!\n");
#elif defined __GNUC__
  printf("Hello world from gcc!\n");
#elif defined _MSC_VER
  printf("Hello world from Visual C!\n");
#else
  printf("Hello world from somethig else!\n");
#endif
```

# Реализация

- Чтение PE файла и вывод одной его секции в файл. Формат использования такой:

```
./section-extracter <source-pe-file> <section-name> <output-bin-image>
```

- Код размещается в директории `solution/src`, заголовочные файлы в `solution/include`.
- Код портабельный и корректно работает с компиляторами gcc, clang, visual C.
- Код документирован в формате `doxygen`.
