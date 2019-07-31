#include <stdint.h>

#ifndef SXT_READER_SXTPARSER_H
    #define SXT_READER_SXTPARSER_H
#endif //SXT_READER_SXTPARSER_H

struct SXT_Document * parseSXTFile(const struct SXT_File *buffer);
void printSXTBody(struct SXT_Document *document);