#ifndef _FILEREADER_GZIP_H_
#define _FILEREADER_GZIP_H_

#include "filereader_core.hpp"
#include <zlib.h>

class GzipFileReader : public FileReader
{
public:
    GzipFileReader();
    virtual ~GzipFileReader();

    virtual bool open(int fd);
    virtual bool seek(off_t offset);
    virtual off_t tell(void);
    virtual bool eof(void);
    virtual int getc();

private:
    gzFile m_file;
};


#endif /* _FILEREADER_GZIP_H_ */
