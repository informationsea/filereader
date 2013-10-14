#include "filereader_gzip.hpp"

GzipFileReader::GzipFileReader() :
    m_file(0)
{

}

GzipFileReader::~GzipFileReader()
{
    if (m_file)
        gzclose(m_file);
}

bool GzipFileReader::open(int fd)
{
    m_file = gzdopen(fd, "r");
    if (m_file)
        return true;
    return false;
}

bool GzipFileReader::seek(off_t offset)
{
    z_off_t ret = gzseek(m_file, offset, SEEK_SET);
    return ret >= 0 ? true : false;
}

off_t GzipFileReader::tell(void)
{
    return gztell(m_file);
}

bool GzipFileReader::eof(void)
{
    return gzeof(m_file);
}

int GzipFileReader::getc()
{
    return gzgetc(m_file);
}


