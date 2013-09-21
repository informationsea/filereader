#include "filereader.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

FileReader::FileReader()
{

}

FileReader::~FileReader()
{
    
}


bool FileReader::open_path(const char* path)
{
    int fd = ::open(path, O_RDONLY);
    if (fd < 0) {
        m_errorno = errno;
        return false;
    }

    m_path = path;

    return open(fd);
}

void FileReader::perror(const char* message)
{
    fprintf(stderr, "%s : %s\n", message, strerror(m_errorno));
}

bool FileReader::seek(off_t offset)
{
    (void)offset;
    return false;
}

#define MINIMUM_BUFFER_SIZE 1024

StdioFileReader::StdioFileReader() :
    m_file(0), m_buffer(0), m_buffer_size(0)
{
    
}

StdioFileReader::~StdioFileReader()
{
    if (m_file)
        fclose(m_file);
    if (m_buffer)
        free(m_buffer);
}

bool StdioFileReader::open(int fd)
{
    m_file = fdopen(fd, "rb");
    if (m_file == NULL) {
        m_errorno = errno;
        return false;
    }
    
    m_buffer = (char *)malloc(MINIMUM_BUFFER_SIZE);
    if (m_buffer == NULL) {
        fclose(m_file);
        m_file = NULL;
        m_errorno = errno;
        return false;
    }
    
    m_buffer_size = MINIMUM_BUFFER_SIZE;
    
    return true;
}

bool StdioFileReader::seek(off_t offset)
{
    int ret = fseeko(m_file, offset, SEEK_SET);
    return ret == 0 ? true : false;
}

const char* StdioFileReader::read(size_t length, size_t *readlen)
{
    if (length > m_buffer_size) {
        char *newbuf = (char *)realloc(m_buffer, length + MINIMUM_BUFFER_SIZE);
        if (newbuf == NULL) {
            m_errorno = errno;
            return NULL;
        }

        m_buffer = newbuf;
    }

    size_t s = fread(m_buffer, length, 1, m_file);

    if (readlen != NULL) {
        *readlen = s;
    }
    return m_buffer;
}














