#ifndef _CSVREADER_H_
#define _CSVREADER_H_

#include <sys/types.h>
#include "tablereader.hpp"
#include "growbuffer.hpp"

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
    char *m_buffer;
    size_t m_bufferSize;
    size_t m_bufferPoint;
};


#endif /* _CSVREADER_H_ */
