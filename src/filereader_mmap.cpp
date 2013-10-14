#include "filereader_mmap.hpp"

#ifndef _WIN32

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

MmapReader::MmapReader() :
    m_mapped_buffer(0),
    m_mapped_length(0), m_offset(0)
{
    
}

MmapReader::~MmapReader()
{
    if (m_mapped_buffer)
        munmap(m_mapped_buffer, m_mapped_length);
    if (m_fd)
        close(m_fd);
}

bool MmapReader::open(int fd)
{
    m_fd = fd;

    struct stat filestat;
    int ret = fstat(fd, &filestat);
    if (ret != 0) {
        m_errorno = errno;
        return false;
    }

    m_filesize = filestat.st_size;

    m_mapped_buffer = (char *)mmap(NULL, m_filesize, PROT_READ, MAP_FILE|MAP_PRIVATE, m_fd, 0);
    if (m_mapped_buffer == MAP_FAILED) {
        m_mapped_buffer = NULL;
        m_errorno = errno;
        return false;
    }
    m_mapped_length = m_filesize;

    return true;
}

const char* MmapReader::read(size_t length, size_t *readlen)
{
    if (m_offset + length > m_mapped_length) {
        length = m_mapped_length - m_offset;
    }

    *readlen = length;
    off_t original_offset = m_offset;
    m_offset += length;

    return m_mapped_buffer + original_offset;
}

bool MmapReader::seek(off_t offset)
{
    if (offset < 0 || offset > m_filesize)
        return false;
    m_offset = offset;
    return true;
}

int MmapReader::getc()
{
    if (m_offset >= m_filesize)
        return -1;
    return m_mapped_buffer[m_offset++];
}

const char* MmapReader::readblock(size_t *readlen, isdelimiter_func f, void* userobj)
{
    size_t i;
    char *pointer = m_mapped_buffer + m_offset;

    for (i = 0; i < m_mapped_length - m_offset; ++i) {
        if (f(*pointer, userobj)) {
            i++;
            break;
        }
        pointer++;
    }

    *readlen = i;
    off_t original_offset = m_offset;
    m_offset += *readlen;
    return m_mapped_buffer + original_offset;
}

#endif
