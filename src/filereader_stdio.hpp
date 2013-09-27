#ifndef _FILEREADER_STDIO_H_
#define _FILEREADER_STDIO_H_

#include "filereader_core.hpp"

class StdioFileReader : public FileReader
{
public:
    StdioFileReader();
    virtual ~StdioFileReader();

    virtual bool open(int fd);
    virtual bool seek(off_t offset);
    virtual int getc();

private:
    FILE *m_file;
};


#endif /* _FILEREADER_STDIO_H_ */

