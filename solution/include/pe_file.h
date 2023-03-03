#ifndef PE_FILE_H
#define PE_FILE_H

#include <stdint.h>

#include "io_utils.h"

struct
#ifdef __clang__
    __attribute__((packed))
#elif defined __GNUC__
    __attribute__((packed))
#elif defined _MSC_VER
#pragma pack(push, 1)
#endif
    PEHeader {
    uint16_t machine;
    uint16_t number_of_sections;
    uint32_t time_date_stamp;
    uint32_t pointer_to_symbol_table;
    uint32_t number_of_symbols;
    uint16_t size_of_optional_header;
    uint16_t characteristics;
};
#ifdef _MSC_VER
#pragma pack(pop)
#endif

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

struct PESection {
    char* name;
    uint32_t size;
    char* data;
    struct PEFile* pe_file;
};

my_reader read_pe_file;
void pe_file_destroy(struct PEFile* pe_file);

my_reader read_pe_section;
void pe_section_destroy(struct PESection* pe_section);

my_writer write_pe_section;

#endif
