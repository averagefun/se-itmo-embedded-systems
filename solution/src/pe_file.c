#include "pe_file.h"

#include <malloc.h>
#include <stdio.h>
#include <string.h>

#define MS_DOS_STUB_OFFSET 0x3c

struct
#ifdef __clang__
    __attribute__((packed))
#elif defined __GNUC__
    __attribute__((packed))
#elif defined _MSC_VER
#pragma pack(push, 1)
#endif
    SectionHeader {
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
#ifdef _MSC_VER
#pragma pack(pop)
#endif

static struct SectionHeader* get_section(struct SectionHeader* section_headers,
                                         size_t n, char* by_name) {
    for (size_t i = 0; i < n; i++) {
        if (strcmp(section_headers[i].name, by_name) == 0) {
            return &section_headers[i];
        }
    }
    return NULL;
}

enum read_status read_pe_file(FILE* source, void* content) {
    if (source == NULL && content == NULL) {
        return READ_INVALID_FILE;
    }

    struct PEFile* pe_file = (struct PEFile*)content;

    if (fseek(source, MS_DOS_STUB_OFFSET, SEEK_SET))
        return READ_INVALID_SIGNATURE;

    if (fread(&pe_file->magic_offset, sizeof(pe_file->magic_offset), 1,
              source) != 1)
        return READ_INVALID_SIGNATURE;

    if (fseek(source, pe_file->magic_offset, SEEK_SET))
        return READ_INVALID_SIGNATURE;

    if (fread(&pe_file->magic, sizeof(pe_file->magic), 1, source) != 1) {
        return READ_INVALID_SIGNATURE;
    }

    if (fread(&pe_file->header, sizeof(pe_file->header), 1, source) != 1)
        return READ_INVALID_HEADER;

    if (fseek(source, pe_file->header.size_of_optional_header, SEEK_CUR))
        return READ_INVALID_SIGNATURE;

    pe_file->section_headers = malloc(sizeof(struct SectionHeader) *
                                      pe_file->header.number_of_sections);
    if (fread(pe_file->section_headers, sizeof(struct SectionHeader),
              pe_file->header.number_of_sections,
              source) != pe_file->header.number_of_sections)
        return READ_INVALID_SIGNATURE;

    return READ_OK;
}

void pe_file_destroy(struct PEFile* pe_file) { free(pe_file->section_headers); }

enum read_status read_pe_section(FILE* source, void* content) {
    if (source == NULL && content == NULL) {
        return READ_INVALID_FILE;
    }

    struct PESection* pe_section = (struct PESection*)content;
    if (pe_section == NULL) {
        return READ_INVALID_SIGNATURE;
    }

    struct SectionHeader* section_header = get_section(
        pe_section->pe_file->section_headers,
        pe_section->pe_file->header.number_of_sections, pe_section->name);

    if (fseek(source, (intptr_t)section_header->pointer_to_raw_data, SEEK_SET))
        return READ_INVALID_SIGNATURE;

    char* raw_data = malloc(section_header->size_of_raw_data);
    if (fread(raw_data, section_header->size_of_raw_data, 1, source) != 1)
        return READ_INVALID_HEADER;

    pe_section->size = section_header->size_of_raw_data;
    pe_section->data = raw_data;

    return READ_OK;
}

void pe_section_destroy(struct PESection* pe_section) {
    free(pe_section->data);
}

enum write_status write_pe_section(FILE* output, void* content) {
    if (output == NULL && content == NULL) {
        return WRITE_INVALID_FILE;
    }

    struct PESection* pe_section = (struct PESection*)content;

    fwrite(pe_section->data, pe_section->size, 1, output);

    // ferror -> 0 if success
    if (ferror(output) != 0) {
        return WRITE_DATA_FAILED;
    }

    return WRITE_OK;
}
