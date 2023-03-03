/// @file
/// @brief Header file describing some structs and IO operations with PE files

#ifndef PE_FILE_H
#define PE_FILE_H

#include <stdint.h>

#include "io_utils.h"

/// Structure containing PE header
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

/// Structure containing PE file data
struct PEFile {
    /// Offset to a file magic
    uint32_t magic_offset;

    /// File magic
    uint32_t magic;

    /// Main header
    struct PEHeader header;

    /// Array of section headers with the size of header.number_of_sections
    struct SectionHeader* section_headers;
};

// Structure containtin PE section and pointer to its PE file
struct PESection {
    char* name;
    uint32_t size;
    char* data;
    struct PEFile* pe_file;
};

/// @brief Read header of PE file and fill PEFile* struct
///
/// @param source Source PE file
/// @param content PEFile* struct to fill
/// @return enum read_status
my_reader read_pe_file;

/// @brief Destroy memory allocated in PEFile*
///
/// @param pe_file PE File to destroy
void pe_file_destroy(struct PEFile* pe_file);

/// @brief Read section of pe file
///
/// @param source Source PE file
/// @param content PESection* struct that'll fill with data
/// @return enum read_status
my_reader read_pe_section;

/// @brief Destroy memory allocated in PESection*
///
/// @param pe_section Section to destroy
void pe_section_destroy(struct PESection* pe_section);

/// @brief Write section to file
///
/// @param output File to write section
/// @param content PESection* struct that contains section data
/// @return enum write_status
my_writer write_pe_section;

#endif
