/// @file
/// @brief Implements input-output operation utils

#include "io_utils.h"

#define READ_ERROR_STRING(ERR) "Error while reading from file: " #ERR "!\n"
#define WRITE_ERROR_STRING(ERR) "Error while writing to file: " #ERR "!\n"

char* read_status_msgs_en[] = {
    [READ_OK] = "File readed succsesfully :)\n",
    [READ_INVALID_FILE] = READ_ERROR_STRING("invalid filename"),
    [READ_INVALID_SIGNATURE] = READ_ERROR_STRING("invalid file signature"),
    [READ_INVALID_HEADER] = READ_ERROR_STRING("invalid header")};

char* write_status_msgs_en[] = {
    [WRITE_OK] = "File writed succsesfully :)\n",
    [WRITE_INVALID_FILE] = WRITE_ERROR_STRING("invalid filename"),
    [WRITE_HEADER_FAILED] = WRITE_ERROR_STRING("invalid header"),
    [WRITE_DATA_FAILED] = WRITE_ERROR_STRING("invalid data")};

enum read_status read_file(const char* input_path, my_reader* reader,
                           void* content) {
    FILE* input_file = fopen(input_path, "rb");
    if (!input_file) {
        return READ_INVALID_FILE;
    }
    enum read_status read_status = reader(input_file, content);
    fclose(input_file);
    return read_status;
}

enum write_status write_file(const char* output_path, my_writer* writer,
                             void* content) {
    FILE* output_file = fopen(output_path, "wb");
    if (!output_file) {
        return WRITE_INVALID_FILE;
    }
    enum write_status write_status = writer(output_file, content);
    fclose(output_file);
    return write_status;
}
