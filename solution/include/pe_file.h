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

struct __attribute__((packed)) SectionHeader {
    char name[8];
    uint32_t virtual_size;
    uint32_t virtual_address;
    uint32_t size_of_raw_data;
    uint32_t pointer_to_raw_data;
    uint32_t pointer_to_relocations;
    uint32_t pointer_to_linenumbers;
    uint16_t number_of_relocations;
    uint16_t number_of_linenumbers;
    uint32_t characteristics;
};

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

struct PESection {
    char* name;
    struct PEFile* pe_file;
    struct SectionHeader section_header;
    char* data;
};

my_reader read_pe_file;
my_reader read_pe_section;
my_writer write_pe_section;

#endif
