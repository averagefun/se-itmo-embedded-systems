#ifndef PE_FILE_H
#define PE_FILE_H

#include <stdint.h>

#include "io_utils.h"

struct __attribute__((packed)) PEHeader {
    uint16_t machine;
    uint16_t number_of_sections;
    uint32_t time_date_stamp;
    uint32_t pointer_to_symbol_table;
    uint32_t number_of_symbols;
    uint16_t size_of_optional_header;
    uint16_t characteristics;
};

struct SectionHeader;

// Structure containing PE file data
struct PEFile {
    /// Offset to a file magic
    uint32_t magic_offset;

    /// File magic
    uint32_t magic;
    /// Main header
    struct PEHeader header;

    /// Array of section headers with the size of header.number_of_sections
    struct SectionHeader* section_headers;
    ///@}
};

my_reader read_pe_file;
my_writer write_pe_file;

#endif