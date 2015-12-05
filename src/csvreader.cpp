#include "csvreader.hpp"
#include "filereader.hpp"

#include "string.h"

#define START        0
#define DEFAULT      1
#define IN_QUOTE     2
#define ENDING_QUOTE 3
#define END          4

#define DEFAULT_BUFFER_SIZE 1024*10

CSVReader::CSVReader()
{

}

CSVReader::~CSVReader()
{

}

bool CSVReader::open(FileReader * /* newfilreader */)
{
    return true;

}

bool CSVReader::eof()
{
    return false;
}

off_t CSVReader::tell()
{
    return m_filereader->tell();
}

#ifdef _WIN32
char * strndup(const char *s1, size_t n)
{
    char *buf = (char *)malloc(n+1);
    memcpy(buf, s1, n+1);
    buf[n] = '\0';
    return buf;
}

#endif

const char* CSVReader::readnext(size_t *readlen, bool *islinelast)
{

}

