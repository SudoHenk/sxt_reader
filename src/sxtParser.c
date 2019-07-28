#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "sxtFileLoader.h"

// File layout is the following:
// 0,1      --> Magic number (must be 0x41, 0x42)
// 2        --> version
// 3..18    --> creator name
//    (optionally) Admintag
// 19..29   --> tagName (9 chars)
// 30..40   --> tagValue (9 chars)
//
// 41       --> contentLength
// 42 .. x  --> contentBuffer

struct SXT_Fileproperties {
    uint8_t version;
    char creator[16];
    bool isAdmin;
};

struct SXT_Admintag {
    char name[10];
    char value[10];
};

// A document will always consist of a fileheader, optionally a admintag
struct SXT_Document {
    struct SXT_Fileproperties *properties_ptr;
    struct SXT_Admintag *admintag_ptr;
    uint8_t contentLength;
    uint8_t *contentBuffer;
};

/**
 * Validates if the first two bytes are the correct file header.
 * Loads data according to header.
 * @param buffer
 */
void validateMagicNumber(const uint8_t *buffer) {
    // The magic number is "AB"
    if(buffer[0] != 0x41 && buffer[1] != 0x42) {
        printf("UNSUPPORTED FILE HEADER.\n");
        exit(1);
    }
}

/**
 * Parses fileheader.
 * @param buffer
 * @return
 */
struct SXT_Fileproperties * parseSXTFileHeader(const uint8_t *buffer) {
    // Magic number check
    validateMagicNumber(buffer);

    // Load properties into struct
    struct SXT_Fileproperties *properties = calloc(1, sizeof(struct SXT_Fileproperties));
    // Unsafe use of allocated heap, might be NULL.

    // Set file properties
    properties->version = buffer[2];
    // Unsafe operation: Null terminator not guaranteed to be within struct size.
    // Buffer over-write
    strcpy(properties->creator, (char*)buffer + 3);

    if(strcmp(properties -> creator, "admin") == 0) {
        properties -> isAdmin = true;
    }

    return properties;
}


/**
 * Parse the admin tag section.
 * @param buffer
 * @return
 */
struct SXT_Admintag * parseSXTAdminTag(const uint8_t *buffer) {
    struct SXT_Admintag *admintag = calloc(1, sizeof(struct SXT_Admintag));
    strcpy(admintag->name, (char*)buffer);
    strcpy(admintag->value, (char*)buffer + 10);
    return admintag;
}

/**
 * Parse the body of the file.
 * @param buffer
 * @return
 */
uint8_t * parseSXTBody(const struct SXT_File *sxtFile, uint8_t bufferOffset) {
    uint8_t length = (sxtFile->buffer + bufferOffset)[0];
    // Potentially unsafe: allocate a buffer according to user input
    // Buffer over-read
    uint8_t * loc = malloc(length);
    // Only copy the body of the file, minus the header, admintag and the length.
    // Unsafe: buffer over-read possible
    memcpy(loc,  (sxtFile->buffer + bufferOffset+1), sxtFile->fsize-bufferOffset-1);
    return loc;
}


void printSXTFileProperties(struct SXT_Fileproperties *properties) {
    printf("=========================\n");
    printf("Header information:\n");
    printf("=========================\n");
    printf("version: %u\n", properties->version);
    // Potentially unsafe: no null terminator
    printf("creator name: %s\n", properties->creator);
    printf("isAdmin: %u\n", properties->isAdmin);
}

void printSXTAdminTag(struct SXT_Admintag *admintag) {
    printf("=========================\n");
    printf("Admintag information:\n");
    printf("=========================\n");
    // Potentially unsafe: no null terminator
    printf("name: %s\n", admintag->name);
    printf("value: %s\n", admintag->value);
}

void printSXTBody(struct SXT_Document *document) {
    printf("=========================\n");
    printf("Body:\n");
    printf("=========================\n");
    printf("length: %u\n", document->contentLength);
    for (int i = 0; i < document->contentLength; i++)
    {
        printf("%02X ", document->contentBuffer[i]);
    }
    printf("\n");
}

void printSXTDocument(struct SXT_Document *document) {
    printSXTFileProperties(document->properties_ptr);
    if(document->properties_ptr->isAdmin == true) {
        printSXTAdminTag(document->admintag_ptr);
    }
    printSXTBody(document);
}


/**
 * Method that can be called to parse a whole file.
 * @param buffer
 * @return
 */
struct SXT_Document * parseSXTFile(const struct SXT_File  *sxtFile) {
    struct SXT_Document * document = malloc(sizeof(struct SXT_Document));

    struct SXT_Fileproperties * properties = parseSXTFileHeader(sxtFile->buffer);
    document -> properties_ptr = properties;
    printSXTFileProperties(document->properties_ptr);

    uint8_t bufferOffset = 20;
    if(properties->isAdmin == true) {
        struct SXT_Admintag * admintag = parseSXTAdminTag(sxtFile->buffer + bufferOffset);
        document->admintag_ptr = admintag;
        printSXTAdminTag(document->admintag_ptr);
        bufferOffset += 18;
    }
    document->contentLength = (uint8_t) (sxtFile->buffer + bufferOffset)[0];
    document->contentBuffer = parseSXTBody(sxtFile, bufferOffset);

    printSXTBody(document);

    return document;
}