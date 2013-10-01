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
    m_file = fdopen(fd, "rb");
    if (m_file == NULL) {
        m_errorno = errno;
        return false;
    }
    m_fd = fd;
    
    return true;
}

bool StdioFileReader::seek(off_t offset)
{
    int ret = fseeko(m_file, offset, SEEK_SET);
    return ret == 0 ? true : false;
}

off_t StdioFileReader::tell(void)
{
    return ftello(m_file);
}

int StdioFileReader::getc()
{
    return fgetc(m_file);
}
