#include "csvreader.hpp"
#include "filereader.hpp"

#include "string.h"

CSVReader::CSVReader() : m_state(CSV_PARSER_INITIAL)
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
    return m_filereader->eof();
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
    do {
        int ch = m_filereader->getc();
        if (ch < 0) {
            switch (m_state) {
            case CSV_PARSER_INITIAL:
            case CSV_PARSER_ENDING_LINE:
                return NULL;
            default:
                m_state = CSV_PARSER_INITIAL;
                *islinelast = true;
                goto onfinish;
            }
        }

        //fprintf(stderr, "CHAR[%02x %c] STATE:%d BUFFER_LEN: %zu\n", ch, ch, m_state, m_buffer.size());
        
        switch (m_state) {
        case CSV_PARSER_ENDING_LINE: {
            if (ch == '\n') {
                m_state = CSV_PARSER_INITIAL;
                break;
            }
            // no break
        }
        case CSV_PARSER_INITIAL: {
            m_buffer.cleanAndShrink();
            if (ch == '"') {
                m_state = CSV_PARSER_IN_QUOTE;
                break;
            }
            // no break
        }
        case CSV_PARSER_IN_CELL: {
            switch (ch) {
            case ',':
                *islinelast = false;
                m_state = CSV_PARSER_INITIAL;
                goto onfinish;
            case '\r':
                m_state = CSV_PARSER_ENDING_LINE;
                *islinelast = true;
                goto onfinish;
                break;
            case '\n':
                *islinelast = true;
                m_state = CSV_PARSER_INITIAL;
                goto onfinish;
            default:
                m_state = CSV_PARSER_IN_CELL;
                m_buffer.append(ch);
                break;
            }
            break;
        }
        case CSV_PARSER_IN_QUOTE: {
            switch (ch) {
            case '"':
                m_state = CSV_PARSER_ENDING_QUOTE;
                break;
            default:
                m_buffer.append(ch);
                break;
            }
            break;
        }
        case CSV_PARSER_ENDING_QUOTE: {
            switch (ch) {
            case '"':
                m_buffer.append("\"\"", 2);
                m_state = CSV_PARSER_IN_QUOTE;
                break;
            case ',':
                *islinelast = false;
                m_state = CSV_PARSER_INITIAL;
                goto onfinish;
            case '\r':
                m_state = CSV_PARSER_ENDING_LINE;
                *islinelast = true;
                goto onfinish;
                break;
            case '\n':
                *islinelast = true;
                m_state = CSV_PARSER_INITIAL;
                goto onfinish;
            default:
                m_state = CSV_PARSER_IN_CELL;
                m_buffer.append(ch);
                break;
            }
            break;
        }
        }
        
    } while(1);

onfinish:
    m_buffer.normalizeQuote();
    m_buffer.append('\0');
    *readlen = m_buffer.size() - 1; // without terminator
    //fprintf(stderr, "RETURN : %s\n", m_buffer.data());
    return m_buffer.data();
}

