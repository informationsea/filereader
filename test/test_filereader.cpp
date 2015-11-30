#include <filereader.hpp>
#include "test_common.hpp"

static void fileread_test(FileReader *reader, const char *path = "TESTFILE", bool checklength = true) {
    ASSERT_TRUE(reader->open_path(path));
    if (checklength)
        ASSERT_EQ(93, reader->length());
        
    size_t readlen;
    const char* buf = reader->read(16, &readlen);
    ASSERT_TRUE(NULL != buf);
    ASSERT_EQ(16, readlen);
    ASSERT_MEMEQ("0123456789ABCDEF", 16, buf, readlen);

    ASSERT_EQ(true, reader->seek(32));
    buf = reader->read(16, &readlen);
    ASSERT_TRUE(NULL != buf);
    ASSERT_MEMEQ("0123456789ABCDEF", 16, buf, readlen);

    ASSERT_EQ(32+16, reader->tell());

    ASSERT_EQ(true, reader->seek(80));
    buf = reader->read(16, &readlen);
    ASSERT_TRUE(NULL != buf);
    ASSERT_MEMEQ("Test appedix\n", 13, buf, readlen);

    ASSERT_TRUE(reader->eof());
}

static void readline_test(FileReader *reader, const char *path = "TESTFILE") {
    const char* test_file_path = path;
    ASSERT_EQ(true, reader->open_path(test_file_path));

    char* buf = reader->readline_dup();
    ASSERT_STREQ("0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF\n", buf);
    free(buf);

    buf = reader->readline_dup();
    ASSERT_STREQ("This is a pen.\n", buf);
    free(buf);

    buf = reader->readline_dup();
    ASSERT_STREQ("Test appedix\n", buf);
    free(buf);
    
    buf = reader->readline_dup();
    ASSERT_STREQ("", buf);
    free(buf);

    ASSERT_TRUE(reader->eof());
}

TEST(READER, stdio_fileread)
{
    StdioFileReader *reader = new StdioFileReader();
    fileread_test(reader);
    delete reader;
}

TEST(READER, gzip_readline)
{
    StdioFileReader *reader = new StdioFileReader();
    readline_test(reader);
    delete reader;
}

TEST(READER, gzip_fileread)
{
    GzipFileReader *reader = new GzipFileReader();
    fileread_test(reader, "TESTFILE.gz", false);
    delete reader;
}

TEST(READER, _stdio_readline)
{
    StdioFileReader *reader = new StdioFileReader();
    readline_test(reader);
    delete reader;
}

TEST(READER, mmap_filereader)
{
    MmapReader *reader = new MmapReader();
    fileread_test(reader);
    delete reader;
}

TEST(READER, mmap_readline)
{
    MmapReader *reader = new MmapReader();
    readline_test(reader);
    delete reader;
}
