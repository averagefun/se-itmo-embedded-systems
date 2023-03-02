#include <stdbool.h>
#include <stdio.h>

#include "io_utils.h"
#include "pe_file.h"

#define APP_NAME "section-extractor"

void usage(FILE* f) {
    fprintf(f, "Usage: " APP_NAME " <in_file> <section_name> <out_file>\n");
}

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;  // supress 'unused parameters' warning

    if (argc != 4) {
        usage(stdout);
        return 1;
    }

    struct PEFile pe_file;
    enum read_status read_status = read_file(argv[1], read_pe_file, &pe_file);

    if (read_status == READ_OK) {
        fprintf(stdout, "%s", read_status_msgs_en[READ_OK]);
    } else {
        fprintf(stderr, "%s", read_status_msgs_en[read_status]);
        return false;
    }

    enum write_status write_status =
        write_file(argv[3], write_pe_file, &pe_file);

    if (write_status == WRITE_OK) {
        fprintf(stdout, "%s", write_status_msgs_en[WRITE_OK]);
    } else {
        fprintf(stderr, "%s", write_status_msgs_en[write_status]);
        return false;
    }
}
