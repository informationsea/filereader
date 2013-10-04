#ifndef _CSVREADER_H_
#define _CSVREADER_H_

#include <sys/types.h>
#include "tablereader.hpp"

class CSVReader : public TableReader
{
public:
    CSVReader();
    virtual ~CSVReader();

    virtual const char* readnext(size_t *readlen, bool *islinelast);
    virtual bool eof();
    virtual off_t tell();

protected:
    virtual bool open(FileReader *newfilreader);
    
private:
    const char* readnext_in_buffer(size_t *readlen, bool *islinelast,
                                   bool *endofbuffer, bool *incomplete_quote, bool *should_copy,
                                   int *state);

    bool copyQuotedBuffer(const char *start, size_t length, size_t *newlength);

    bool resizeBuffer(size_t newsize);
    bool resizeBuffer2(size_t newsize);

    off_t m_filereader_buffer_offset;
    const char *m_filereader_buffer;
    size_t m_filereader_buffer_size;

    off_t m_current_offset;
    
    char *m_buffer;
    size_t m_buffer_size;
    char *m_buffer2;
    size_t m_buffer2_size;
};


#endif /* _CSVREADER_H_ */
