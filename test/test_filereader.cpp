#include <cppcutter.h>
#include <filereader.hpp>

namespace stdiotest {

    static void fileread_test(FileReader *reader) {
        cut_assert_equal_int(true, reader->open_path("TESTFILE"));
        size_t readlen;
        const char* buf = reader->read(16, &readlen);
        cut_assert_equal_int(16, readlen);
        cut_assert_equal_memory("0123456789ABCDEF", 16, buf, readlen);

        cut_assert_equal_int(true, reader->seek(32));
        buf = reader->read(16, &readlen);
        cut_assert_equal_memory("0123456789ABCDEF", 16, buf, readlen);

        cut_assert_equal_int(true, reader->seek(80));
        buf = reader->read(16, &readlen);
        cut_assert_equal_memory("Test appedix\n", 13, buf, readlen);
    }

    static void readline_test(FileReader *reader) {
        const char* test_file_path = cut_build_fixture_path(".", "TESTFILE", NULL);
        cut_assert_equal_int(true, reader->open_path(test_file_path));

        char* buf = reader->readline_dup();
        cut_assert_equal_string("0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF\n", buf);
        free(buf);

        buf = reader->readline_dup();
        cut_assert_equal_string("This is a pen.\n", buf);
        free(buf);

        buf = reader->readline_dup();
        cut_assert_equal_string("Test appedix\n", buf);
        free(buf);
    
        buf = reader->readline_dup();
        cut_assert_equal_string("", buf);
        free(buf);
    }

    void test_stdio_fileread(void)
    {
        StdioFileReader *reader = new StdioFileReader();
        fileread_test(reader);
        delete reader;
    }

    void test_stdio_readline(void)
    {
        StdioFileReader *reader = new StdioFileReader();
        readline_test(reader);
        delete reader;
    }

    void test_mmap_filereader(void)
    {
        MmapReader *reader = new MmapReader();
        fileread_test(reader);
        delete reader;
    }

    void test_mmap_readline(void)
    {
        MmapReader *reader = new MmapReader();
        readline_test(reader);
        delete reader;
    }
}
