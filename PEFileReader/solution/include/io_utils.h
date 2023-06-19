/// @file
/// @brief Header file describing input-output operation utils

#ifndef IO_UTILS_H
#define IO_UTILS_H

#include <stdio.h>

extern char* read_status_msgs_en[];
extern char* write_status_msgs_en[];

enum read_status {
    READ_OK = 0,
    READ_INVALID_FILE,
    READ_INVALID_SIGNATURE,
    READ_INVALID_HEADER
};

enum write_status {
    WRITE_OK = 0,
    WRITE_INVALID_FILE,
    WRITE_HEADER_FAILED,
    WRITE_DATA_FAILED
};

typedef enum read_status(my_reader)(FILE* source, void* content);

/// read from 'source_path' file using from_format function
enum read_status read_file(const char* input_path, my_reader* reader,
                           void* content);

typedef enum write_status(my_writer)(FILE* output, void* content);

/// write to file 'output_path' file using to_format function
enum write_status write_file(const char* output_path, my_writer* writer,
                             void* content);

#endif
