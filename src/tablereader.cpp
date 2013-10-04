#include "tablereader.hpp"
#include "filereader.hpp"

#include <string.h>
#include <errno.h>

TableReader::TableReader() :
    m_filereader(0)
{
    
}

TableReader::~TableReader()
{
    if (m_filereader)
        delete m_filereader;
}

bool TableReader::open_path(const char* path)
{
    m_filereader = filereader_get_suitable_reader(path);
    if (!m_filereader) {
        m_errorno = errno;
        return false;
    }
    return open(m_filereader);
}

bool TableReader::open(int fd)
{
    m_filereader = filereader_get_suitable_reader(fd);
    if (!m_filereader) {
        m_errorno = errno;
        return false;
    }
    return open(m_filereader);
}

bool TableReader::open(FileReader *newfilreader)
{
    return true; // do nothing
}

bool TableReader::eof()
{
    return m_filereader->eof();
}

off_t TableReader::tell()
{
    return m_filereader->tell();
}

void TableReader::perror(const char* message)
{
    fprintf(stderr, "%s : %s\n", message, strerror(m_errorno));
}

const char* TableReader::error_message()
{
    return strerror(m_errorno);
}

static bool isdelimiter(char ch, void* userobj)
{
    char *lastchar = (char *)userobj;
    if (ch == '\n' || ch == '\t') {
        if (lastchar)
            *lastchar = ch;
        
        return true;
    }
    return false;
}

const char* TableReader::readnext(size_t *readlen, bool *islinelast)
{
    if (m_filereader == NULL) {
        return NULL;
    }

    char lastchar;
    const char *buf = m_filereader->readblock(readlen, isdelimiter, &lastchar);

    if (!buf)
        return NULL;
    if (*readlen == 0)
        return NULL;

    *readlen -= 1; // do not include delimiter

    if (lastchar == '\n')
        *islinelast = true;
    else
        *islinelast = false;

    return buf;
}
