#include <stdio.h>
#include <stdlib.h>
#include "sxtFileLoader.h"
#include "sxtParser.h"

// SXT file format is an TXT file, but more complex just because we can.
void printSXTFileInfo(struct SXT_File *sxtFile) {
    printf("DEBUG INFO: (size: %lu bytes)\n", sxtFile->fsize);
    for (int i = 0; i < sxtFile->fsize; i++)
    {
        printf("%02X ", sxtFile->buffer[i]);
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    printf("Welcome to SXT File Reader!\n================\n");
    if(argc < 2) {
        printf("Usage: provide the filename as an argument!\n");
    } else {

        struct SXT_File * sxtFile = readSXTFileInBuffer(argv[1]);
        printSXTFileInfo(sxtFile);
        struct SXT_Document * document = parseSXTFile(sxtFile);
        free(sxtFile->buffer);
        free(sxtFile);
    }
    return 0;
}