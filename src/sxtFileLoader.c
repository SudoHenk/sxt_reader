#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include "sxtFileLoader.h"

/**
 * Reads file into given buffer.
 * @param filename Path of the file.
 * @param buffer Buffer to read into. No boundary checks, because we trust everything.
 */
struct SXT_File * readSXTFileInBuffer(char *filename) {
    if(access(filename, F_OK) == -1) {
        printf("Filename passed as argument could not be opened.\n");
        exit(0);
    }

    FILE *f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    uint8_t *buffer = malloc(fsize);
    fread(buffer, 1, fsize, f);
    fclose(f);

    struct SXT_File *sxtFile = malloc(sizeof(struct SXT_File));

    sxtFile->buffer = buffer;
    sxtFile->fsize = fsize;
    return sxtFile;
}