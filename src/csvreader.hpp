#ifndef _CSVREADER_H_
#define _CSVREADER_H_

#include "tablereader.hpp"

class CSVReader : public TableReader
{
public:
    CSVReader();
    virtual ~CSVReader();

    virtual bool open(int fd);
    virtual const char* readnext(size_t *readlen, bool *islinelast);
    
private:
    char *m_filereader_buffer;
    size_t m_filereader_buffer_size;
    char *m_buffer;
    size_t m_buffer_size;
};


#endif /* _CSVREADER_H_ */
