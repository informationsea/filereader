#include "filereader.hpp"

#include <string.h>

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
    size_t pathlen = strlen(path);
#ifndef DISABLE_GZIP
    if (strncmp(path+pathlen-3, ".gz", 3) == 0) {
        fprintf(stderr, "Gzip\n");
        GzipFileReader *gfr = new GzipFileReader();
        if (gfr->open_path(path))
            return gfr;
        delete gfr;
    }
#endif

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

