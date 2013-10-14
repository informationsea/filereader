#ifndef _FILEREADER_MMAP_H_
#define _FILEREADER_MMAP_H_

#ifndef _WIN32

#include <sys/types.h>

#include "filereader_core.hpp"

class MmapReader : public FileReader
{
public:
    MmapReader();
    virtual ~MmapReader();

    virtual bool open(int fd);
    virtual const char* read(size_t length, size_t *readlen);
    virtual bool seek(off_t offset);
    virtual off_t tell(void) {return m_offset;}
    virtual off_t length(void) {return m_filesize;}
    virtual bool eof(void) {return m_offset == m_filesize;}
    virtual int getc();
    virtual const char* readblock(size_t *readlen, isdelimiter_func f, void* userobj);

private:
    off_t m_filesize;
    char *m_mapped_buffer;
    size_t m_mapped_length;
    off_t m_offset;
};

#endif

#endif /* _FILEREADER_MMAP_H_ */

