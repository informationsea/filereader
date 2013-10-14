#include "filereader_stdio.hpp"

#include <stdio.h>
#include <errno.h>

StdioFileReader::StdioFileReader() :
    m_file(0)
{
    
}

StdioFileReader::~StdioFileReader()
{
    if (m_file)
        fclose(m_file);
}

bool StdioFileReader::open(int fd)
{
#ifdef _WIN32
    m_file = _fdopen(fd, "rb");
#else
    m_file = fdopen(fd, "rb");
#endif
    if (m_file == NULL) {
        m_errorno = errno;
        return false;
    }
    m_fd = fd;
    
    return true;
}

bool StdioFileReader::seek(off_t offset)
{
#ifdef _WIN32
    int ret = _fseeki64(m_file, offset, SEEK_SET);
#else
    int ret = fseeko(m_file, offset, SEEK_SET);
#endif
    return ret == 0 ? true : false;
}

off_t StdioFileReader::tell(void)
{
#ifdef _WIN32
    return _ftelli64(m_file);
#else
    return ftello(m_file);
#endif
}

bool StdioFileReader::eof(void)
{
    return feof(m_file) ? true : false;
}

int StdioFileReader::getc()
{
    return fgetc(m_file);
}
