#ifndef _TABLEREADER_H_
#define _TABLEREADER_H_

#include <stdlib.h>
#include <sys/types.h>

class FileReader;

class TableReader
{
public:
    TableReader();
    virtual ~TableReader();

    virtual bool open_path(const char* path);
    virtual bool open(int fd);
    virtual bool eof();
    virtual off_t tell();

    virtual void perror(const char* message);
    virtual const char* error_message();

    // column text is not null terminated!
    // returned pointer will expire at next readnext call!
    virtual const char* readnext(size_t *readlen, bool *islinelast);

protected:
    FileReader *m_filereader;
    int m_errorno;

    virtual bool open(FileReader *newfilreader);
    
private:
    
    
};

#endif /* _TABLEREADER_H_ */
