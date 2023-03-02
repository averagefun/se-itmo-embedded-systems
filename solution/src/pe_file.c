#include "pe_file.h"

#include <malloc.h>
#include <stdio.h>
#include <string.h>

#define MS_DOS_STUB_OFFSET 0x3c

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

    // fseek -> 0 if success
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
              source) != pe_file->header.number_of_sections) {
        return READ_INVALID_SIGNATURE;
    }

        return READ_OK;
}

enum write_status write_pe_file(FILE* source, FILE* output, void* content,
                                char* key) {
    if (output == NULL && content == NULL) {
        return WRITE_INVALID_FILE;
    }

    struct PEFile* pe_file = (struct PEFile*)content;

    struct SectionHeader* section_header = get_section(
        pe_file->section_headers, pe_file->header.number_of_sections, key);

    if (section_header == NULL) {
        return WRITE_HEADER_FAILED;
    }

    if (fseek(source, section_header->pointer_to_raw_data, SEEK_SET))
        return WRITE_HEADER_FAILED;

    void* raw_data = malloc(section_header->size_of_raw_data);
    if (fread(raw_data, section_header->size_of_raw_data, 1, source) != 1)
        return WRITE_DATA_FAILED;

    fwrite(raw_data, section_header->size_of_raw_data, 1, output);

    // ferror -> 0 if success
    if (ferror(output) != 0) {
        return WRITE_DATA_FAILED;
    }

    free(raw_data);

    return WRITE_OK;
}