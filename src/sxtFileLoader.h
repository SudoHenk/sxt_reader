#include <stdint.h>

#ifndef SXT_READER_FILELOADER_H
    #define SXT_READER_FILELOADER_H
#endif //SXT_READER_FILELOADER_H


struct SXT_File {
    uint8_t * buffer;
    long fsize;
};

struct SXT_File * readSXTFileInBuffer(char *filename);