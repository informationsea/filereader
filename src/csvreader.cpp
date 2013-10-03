#include "csvreader.hpp"
#include "filereader.hpp"

#include "string.h"

#define START        0
#define DEFAULT      1
#define IN_QUOTE     2
#define ENDING_QUOTE 3
#define END          4

#define DEFAULT_BUFFER_SIZE 1024*10

CSVReader::CSVReader() :
    m_filereader_buffer_offset(0),
    m_filereader_buffer(0), m_filereader_buffer_size(0),
    m_current_offset(0),
    m_buffer(0), m_buffer_size(0), m_buffer2(0), m_buffer2_size(0)
{

}

CSVReader::~CSVReader()
{
    if (m_buffer)
        free(m_buffer);
    if (m_buffer2)
        free(m_buffer2);
}

bool CSVReader::open(FileReader */* newfilreader */)
{
    m_filereader_buffer_offset = m_filereader->tell();
    m_filereader_buffer = m_filereader->read(DEFAULT_BUFFER_SIZE, &m_filereader_buffer_size);
    if (m_filereader_buffer == NULL)
        return false;

    m_buffer = (char *)malloc(DEFAULT_BUFFER_SIZE);
    if (m_buffer == NULL)
        return false;
    m_buffer_size = DEFAULT_BUFFER_SIZE;

    m_buffer2 = (char *)malloc(DEFAULT_BUFFER_SIZE);
    if (m_buffer2 == NULL)
        return false;
    m_buffer2_size = DEFAULT_BUFFER_SIZE;

    return true;

}

const char* CSVReader::readnext(size_t *readlen, bool *islinelast)
{
    bool endofbuffer;
    bool incomplete_quote = false;
    bool should_copy = false;
    int state = START;
    const char *value = readnext_in_buffer(readlen, islinelast, &endofbuffer,
                                           &incomplete_quote, &should_copy, &state);
    if (!endofbuffer)
        return value;
    if (m_filereader->eof()) {
        printf("End of file\n");
        return value;
    }

    {
        char *str = strndup(value, *readlen);
        printf("length: %zu %s\n", *readlen, str);
        free(str);
    }
    size_t offset = 0;
    size_t length = *readlen;

    if (m_buffer2_size < *readlen) {
        if (!resizeBuffer2(*readlen))
            return NULL; // failed to allocate memory; TODO: raise error!
    }
    memcpy(m_buffer2, value, *readlen);
    offset += length;

    do {
        m_filereader_buffer_offset = m_filereader->tell();
        m_filereader_buffer = m_filereader->read(DEFAULT_BUFFER_SIZE, &m_filereader_buffer_size);
        m_current_offset = 0;

        value = readnext_in_buffer(readlen, islinelast, &endofbuffer,
                                   &incomplete_quote, &should_copy, &state);

        if (m_buffer2_size < *readlen + length) {
            if (!resizeBuffer2(*readlen + length))
                return NULL; // failed to allocate memory; TODO: raise error!
        }


        {
            char *str = strndup(value, *readlen);
            printf("length: %zu %s\n", *readlen, str);
            free(str);
        }

        memcpy(m_buffer2 + offset, value, *readlen);
        offset = length;
        length += *readlen;
        
    } while(endofbuffer && !m_filereader->eof());

    *readlen = length;

    if (incomplete_quote) {
        *readlen -= 2;

        if (should_copy) {
            size_t newlength;
            if (!copyQuotedBuffer(m_buffer2+1, *readlen, &newlength)) {
                return NULL;
            }
            *readlen = newlength;
            return m_buffer;
        }

        return m_buffer2 + 1;
    }
    
    return m_buffer2;
}

const char* CSVReader::readnext_in_buffer(size_t *readlen, bool *islinelast,
                                          bool *endofbuffer, bool *incomplete_quote,
                                          bool *should_copy, int *state)
{
    off_t buffer_offset = m_current_offset;

    const char *start = m_filereader_buffer + buffer_offset;
    const char *current = start;
    const char *end = m_filereader_buffer + m_filereader_buffer_size;

    *endofbuffer = false;

    if (m_current_offset == m_filereader_buffer_size+1) {
        *readlen = 0;
        *islinelast = false;
        *endofbuffer = true;
        return NULL;
    } else if (m_current_offset == m_filereader_buffer_size && *start == '\n') {
        *readlen = 0;
        *islinelast = false;
        *endofbuffer = true;
        return NULL;
    }

    char lastchar = '\0';
    bool quoted = false;
    
    for (; current < end; current++) {
        //printf("CSV %d : %c\n", *state, *current);
        switch(*state) {
        case START:
            switch(*current) {
            case '"':
                quoted = true;
                *state = IN_QUOTE;
                goto break2;
            case '\n':
                continue;
            }
            *state = DEFAULT;
            // no break at here
        case DEFAULT:
            switch (*current) {
            case ',':
            case '\n':
            case '\r':
                *state = END;
                break;
            default:
                break;
            }
            break;
        case IN_QUOTE:
            if (*current == '"')
                *state = ENDING_QUOTE;
            break;
        case ENDING_QUOTE:
            switch(*current) {
            case '"':
                *state = IN_QUOTE;
                *should_copy = true;
                break;
            case ',':
            case '\n':
            case '\r':
                *state = END;
                break;
            default: // ignore
                *state = IN_QUOTE;
                break;
            }
            break;
        case END:
            break;
        }

      break2:

        lastchar = *current;
        if (*state == END)
            break;
    }

    *readlen = current - start;
    switch(lastchar) {
    case '\n':
    case '\r':
        *islinelast = true;
        break;
    default:
        *islinelast = false;
        break;
    }

    m_current_offset += *readlen + 1;
    if (m_current_offset >= m_filereader_buffer_size) {
        if (m_filereader->eof()) {
            *islinelast = true;
            printf("read next: %d %d %d %zu\n", *state, quoted, *incomplete_quote, *readlen);
        } else {
            *islinelast = true;
            *endofbuffer = true;
            if (quoted && (*state != END || *state != ENDING_QUOTE)) {
                *incomplete_quote = true;
                quoted = false;
            }
        }
    }

    if (quoted && !(*incomplete_quote)) {
        start++;
        *readlen -= 2;
    }

    if (*should_copy && !(*incomplete_quote)) {
        size_t newlength;
        if (!copyQuotedBuffer(start, *readlen, &newlength)) {
            return NULL;
        }
        *readlen = newlength;
        return m_buffer;
    } else {
        return start;
    }
}

bool CSVReader::resizeBuffer(size_t newsize)
{
    char *newbuffer = (char *)realloc(m_buffer, newsize);
    if (newbuffer == NULL)
        return false;
    m_buffer = newbuffer;
    m_buffer_size = newsize;
    return true;
}

bool CSVReader::resizeBuffer2(size_t newsize)
{
    char *newbuffer = (char *)realloc(m_buffer2, newsize);
    if (newbuffer == NULL)
        return false;
    m_buffer2 = newbuffer;
    m_buffer2_size = newsize;
    return true;
}

bool CSVReader::copyQuotedBuffer(const char *start, size_t length, size_t *newlength)
{
    bool last_char_is_quote = false;
    size_t newlength_internal = 0;
    
    const char *end = start + length;
    if (length > m_buffer_size - 1) {
        if (!resizeBuffer(length+1))
            return false;
    }
    char *buffer = m_buffer;
    
    for (const char *current = start; current < end; ++current) {
        switch (*current) {
        case '"':
            if (last_char_is_quote) {
                last_char_is_quote = false;
                break;
            }
            last_char_is_quote = true;
            *buffer++ = *current;
            newlength_internal++;
            break;
        default:
            *buffer++ = *current;
            newlength_internal++;
            last_char_is_quote = false;
        }
    }
    *newlength = newlength_internal;
    return true;
}
