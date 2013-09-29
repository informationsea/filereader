#include "csvreader.hpp"

#define DEFAULT_BUFFER_SIZE

CSVReader::CSVReader() :
    m_filereader_buffer(0), m_filereader_buffer_size(0),
    m_buffer(0), m_buffer_size(0)
{

}

CSVReader::~CSVReader()
{
    if (m_buffer)
        free(m_buffer);
}

bool CSVReader::open(int fd)
{
    return TableReader::open(fd);
}

const char* CSVReader::readnext(size_t *readlen, bool *islinelast)
{
    
}
