#include "filereader.hpp"

FileReader *filereader_get_suitable_reader(int fd)
{
#ifndef _WIN32
    MmapReader *mmapreader = new MmapReader();
    if (mmapreader->open(fd))
        return mmapreader;
    delete mmapreader;
#endif

    StdioFileReader *stdioreader = new StdioFileReader();
    if (stdioreader->open(fd))
        return stdioreader;
    delete stdioreader;
    
    return NULL;
}

FileReader *filereader_get_suitable_reader(const char* path)
{
#ifndef _WIN32
    MmapReader *mmapreader = new MmapReader();
    if (mmapreader->open_path(path))
        return mmapreader;
    delete mmapreader;
#endif

    StdioFileReader *reader = new StdioFileReader();
    if (reader->open_path(path))
        return reader;
    delete reader;
    
    return NULL;
}

