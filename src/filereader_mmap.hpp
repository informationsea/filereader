#ifndef _FILEREADER_MMAP_H_
#define _FILEREADER_MMAP_H_

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
    virtual int getc();
    virtual const char* readblock(size_t *readlen, isdelimiter_func f, void* userobj);

private:
    int m_fd;
    off_t m_filesize;
    char *m_mapped_buffer;
    size_t m_mapped_length;
    off_t m_offset;
};


#endif /* _FILEREADER_MMAP_H_ */

