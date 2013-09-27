#include "filereader_core.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define MINIMUM_BUFFER_SIZE 1024

FileReader::FileReader() :
    m_errorno(0), m_path(0), m_buffer(0), m_buffer_size(0)
{

}

FileReader::~FileReader()
{
    if (m_buffer != NULL)
        free(m_buffer);
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

const char* FileReader::error_message()
{
    return strerror(m_errorno);
}

int FileReader::get_errno()
{
    return m_errorno;
}

bool FileReader::seek(off_t offset)
{
    (void)offset;
    return false;
}

const char* FileReader::read(size_t length, size_t *readlen)
{
    if (length > m_buffer_size) {
        char *newbuf = (char *)realloc(m_buffer, length + MINIMUM_BUFFER_SIZE);
        if (newbuf == NULL) {
            m_errorno = errno;
            return NULL;
        }

        m_buffer = newbuf;
    }

    size_t s = 0;
    for (s = 0; s < length; ++s) {
        int ch = getc();
        if (ch < 0)
            break;
        m_buffer[s] = ch;
    }

    if (readlen != NULL) {
        *readlen = s;
    }
    return m_buffer;
}

static bool isnewline(char ch, void* userobj)
{
    (void)userobj;
    return ch == '\n';
}

const char* FileReader::readline(size_t *readlen)
{
    return readblock(readlen, isnewline, NULL);
}

char* FileReader::readline_dup(size_t *readlen)
{
    size_t duplen;
    const char *bytes = readline(&duplen);
    char *buffer = (char *)malloc(duplen+1);
    memcpy(buffer, bytes, duplen);
    buffer[duplen] = '\0';
    if (readlen != NULL)
        *readlen = duplen+1;
    return buffer;
}

const char* FileReader::readblock(size_t *readlen, isdelimiter_func f, void* userobj)
{
    if (m_buffer == NULL) {
        m_buffer = (char *)malloc(MINIMUM_BUFFER_SIZE);
        if (m_buffer == NULL) {
            m_errorno = errno;
            return NULL;
        }
        m_buffer_size = MINIMUM_BUFFER_SIZE;
    }

    int ch;
    size_t current_pos = 0;
    do {
        ch = getc();
        if (ch < 0)
            break;

        if (current_pos >= m_buffer_size) {
            char *newbuf = (char *)realloc(m_buffer, m_buffer_size*2);
            if (newbuf == NULL) {
                m_errorno = errno;
                free(m_buffer);
                m_buffer = NULL;
                return NULL;
            }
            m_buffer = newbuf;
            m_buffer_size *= 2;
        }
        
        m_buffer[current_pos++] = ch;
        if (f(ch, userobj))
            break;
    } while(1);

    m_buffer[current_pos] = '\0';

    if (readlen)
        *readlen = current_pos;
    
    return m_buffer;
}
