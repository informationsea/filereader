#ifndef _CSVREADER_H_
#define _CSVREADER_H_

#include <sys/types.h>
#include "tablereader.hpp"
#include "growbuffer.hpp"

enum PARSER_STATE {
    CSV_PARSER_INITIAL,
    CSV_PARSER_IN_CELL,
    CSV_PARSER_IN_QUOTE,
    CSV_PARSER_ENDING_QUOTE,
    CSV_PARSER_ENDING_LINE
};


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
    GrowBuffer m_buffer;
    enum PARSER_STATE m_state;
};


#endif /* _CSVREADER_H_ */
