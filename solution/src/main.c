/// @file
/// @brief Main application file

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io_utils.h"
#include "pe_file.h"

#define APP_NAME "section-extractor"

/// @brief Print usage test
/// @param[in] f File to print to (e.g., stdout)
void usage(FILE* f) {
    fprintf(f, "Usage: " APP_NAME " <in_file> <section_name> <out_file>\n");
}

/// @brief Print read_status to stdout/stderr
/// @param read_status Status from reading file
/// @return true in case of READ_OK, else false
bool print_read_status(enum read_status read_status) {
    if (read_status == READ_OK) {
        fprintf(stdout, "%s", read_status_msgs_en[READ_OK]);
        return true;
    } else {
        fprintf(stderr, "%s", read_status_msgs_en[read_status]);
        return false;
    }
}

/// @brief Print write_status to stdout/stderr
/// @param write_status Status from writing file
/// @return true in case of WRITE_OK, else false
bool print_write_status(enum write_status write_status) {
    if (write_status == WRITE_OK) {
        fprintf(stdout, "%s", write_status_msgs_en[WRITE_OK]);
        return true;
    } else {
        fprintf(stderr, "%s", write_status_msgs_en[write_status]);
        return false;
    }
}

/// @brief Application entry point
/// @param[in] argc Number of command line arguments
/// @param[in] argv Command line arguments
/// @return 0 in case of success or error code
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
