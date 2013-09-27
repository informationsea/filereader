#include "filereader.hpp"

FileReader *filereader_get_suitable_reader(int fd)
{
    StdioFileReader *reader = new StdioFileReader();
    if (reader->open(fd))
        return reader;
    delete reader;
    
    return NULL;
}

FileReader *filereader_get_suitable_reader(const char* path)
{
    StdioFileReader *reader = new StdioFileReader();
    if (reader->open_path(path))
        return reader;
    delete reader;
    
    return NULL;
}

