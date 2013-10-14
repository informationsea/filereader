#ifndef _FILEREADER_CORE_H_
#define _FILEREADER_CORE_H_

#include <stdio.h>
#include <sys/types.h>

typedef bool (isdelimiter_func)(char ch, void *userobj);

class FileReader
{
public:
    FileReader();
    virtual ~FileReader();

    virtual bool open(int fd) = 0;
    virtual bool open_path(const char* path);
    
    virtual bool seek(off_t offset); // always fail if not overridden
    virtual off_t tell(void);        // always fail if not overridden
    virtual off_t length(void);
    virtual bool eof(void) = 0;
    virtual const char* read(size_t length, size_t *readlen);
    virtual int getc() = 0;
    
    // returned pointer will expired at next something read call!
    virtual const char* readline(size_t *readlen);
    virtual char* readline_dup(size_t *readlen = NULL);
    virtual const char* readblock(size_t *readlen, isdelimiter_func f, void* userobj);

    virtual void perror(const char* message);
    virtual const char* error_message();
    virtual int get_errno();
    
protected:
    int m_fd; // used only in length()
    int m_errorno;
    const char* m_path;

private:
    char *m_buffer;
    size_t m_buffer_size;
};

#endif /* _FILEREADER_CORE_H_ */
