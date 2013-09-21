#include <cutter.h>
#include <filereader.hpp>

void test_stdio_fileread (void)
{
    StdioFileReader *reader = new StdioFileReader();
    reader->open_path("TESTFILE");
    
    size_t readlen;
    const char* buf = reader->read(16, &readlen);
    cut_assert_equal_memory("0123456789ABCDEF", 16, buf, readlen);

    cut_assert_equal_int(true, reader->seek(32));
    buf = reader->read(16, &readlen);
    cut_assert_equal_memory("0123456789ABCDEF", 16, buf, readlen);
}

