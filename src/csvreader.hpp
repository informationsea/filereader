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

protected:
    virtual bool open(FileReader *newfilreader);
    
private:
    bool copyQuotedBuffer(const char *start, size_t length, size_t *newlength);
    bool resizeBuffer(size_t newsize);

    off_t m_filereader_buffer_offset;
    const char *m_filereader_buffer;
    size_t m_filereader_buffer_size;

    off_t m_current_offset;
    
    char *m_buffer;
    size_t m_buffer_size;
};


#endif /* _CSVREADER_H_ */
