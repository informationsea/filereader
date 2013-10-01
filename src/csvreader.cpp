#include "csvreader.hpp"
#include "filereader.hpp"

#define DEFAULT_BUFFER_SIZE 1024*10

CSVReader::CSVReader() :
    m_filereader_buffer_offset(0),
    m_filereader_buffer(0), m_filereader_buffer_size(0),
    m_buffer(0), m_buffer_size(0), m_current_offset(0)
{

}

CSVReader::~CSVReader()
{
    if (m_buffer)
        free(m_buffer);
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
    return true;

}

const char* CSVReader::readnext(size_t *readlen, bool *islinelast)
{
    off_t buffer_offset = m_current_offset - m_filereader_buffer_offset;

    const char *start = m_filereader_buffer + buffer_offset;
    const char *current = start;
    const char *end = m_filereader_buffer + m_filereader_buffer_size;

    if (m_current_offset == m_filereader_buffer_size+1) {
        *readlen = 0;
        *islinelast = false;
        return NULL;
    } else if (m_current_offset == m_filereader_buffer_size && *start == '\n') {
        *readlen = 0;
        *islinelast = false;
        return NULL;
    }

    enum {
        START,
        DEFAULT,
        IN_QUOTE,
        ENDING_QUOTE,
        END
    } state = START;

    char lastchar = '\0';
    bool should_copy = false;
    bool quoted = false;
    
    for (; current < end; current++) {
        // printf("CSV %d : %c\n", state, *current);
        switch(state) {
        case START:
            switch(*current) {
            case '"':
                quoted = true;
                state = IN_QUOTE;
                goto break2;
            case '\n':
                continue;
            }
            state = DEFAULT;
            // no break at here
        case DEFAULT:
            switch (*current) {
            case ',':
            case '\n':
            case '\r':
                state = END;
                break;
            default:
                break;
            }
            break;
        case IN_QUOTE:
            if (*current == '"')
                state = ENDING_QUOTE;
            break;
        case ENDING_QUOTE:
            switch(*current) {
            case '"':
                state = IN_QUOTE;
                should_copy = true;
                break;
            case ',':
            case '\n':
            case '\r':
                state = END;
                break;
            default: // ignore
                state = IN_QUOTE;
                break;
            }
            break;
        case END:
            break;
        }

      break2:

        lastchar = *current;
        if (state == END)
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
    if (m_current_offset == m_filereader_buffer_size+1) {
        *islinelast = true;
    }

    if (quoted) {
        start++;
        *readlen -= 2;
    }

    if (should_copy) {
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
