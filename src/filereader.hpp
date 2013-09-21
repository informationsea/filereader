#ifndef _FILEREADER_H_
#define _FILEREADER_H_

#include <stdio.h>

class FileReader
{
public:
    FileReader();
    virtual ~FileReader();

    virtual bool open(int fd) = 0;
    virtual bool open_path(const char* path);
    
    virtual bool seek(off_t offset);
    virtual const char* read(size_t length, size_t *readlen) = 0;

    virtual void perror(const char* message);
    
protected:
    int m_errorno;
    const char* m_path;
};

class StdioFileReader : public FileReader
{
public:
    StdioFileReader();
    virtual ~StdioFileReader();

    virtual bool open(int fd);
    virtual bool seek(off_t offset);
    virtual const char* read(size_t length, size_t *readlen);

private:
    FILE *m_file;
    char *m_buffer;
    size_t m_buffer_size;
};

#endif /* _FILEREADER_H_ */
