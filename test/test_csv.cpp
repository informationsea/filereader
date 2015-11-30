#include <csvreader.hpp>
#include <stdio.h>
#include <string.h>
#include "test_common.hpp"    

TEST(CSV,readcsv2) {
    CSVReader *reader = new CSVReader();
    const char* test_file_path = "TESTCSV2.csv";
    ASSERT_TRUE(reader->open_path(test_file_path));

    bool islinelast;
    size_t readlen;
    const char *column;
        
    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("This", 4, column, readlen);
    ASSERT_FALSE(islinelast);
    ASSERT_EQ(5, reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("", 0, column, readlen);
    ASSERT_FALSE(islinelast);
    ASSERT_EQ(6, reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("", 0, column, readlen);
    ASSERT_TRUE(islinelast);
    ASSERT_EQ(7, reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("is", 2, column, readlen);
    ASSERT_FALSE(islinelast);
    ASSERT_EQ(10, reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("a", 1, column, readlen);
    ASSERT_FALSE(islinelast);
    ASSERT_EQ(12, reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("pen", 3, column, readlen);
    ASSERT_TRUE(islinelast);
    ASSERT_EQ(16, reader->tell());

    ASSERT_TRUE(reader->eof());
        
    delete reader;
}

void helper_test_readcsv(CSVReader *reader, bool windowsReturn) {
    bool islinelast;
    size_t readlen;
    const char *column;
        
    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("A", 1, column, readlen);
    ASSERT_FALSE(islinelast);
    ASSERT_EQ(2, reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("B", 1, column, readlen);
    ASSERT_FALSE(islinelast);
    ASSERT_EQ(4, reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("C", 1, column, readlen);
    ASSERT_FALSE(islinelast);
    ASSERT_EQ(6, reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("D", 1, column, readlen);
    ASSERT_TRUE(islinelast);
    ASSERT_EQ(8, reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("This is a pen.", 14, column, readlen);
    ASSERT_FALSE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("test b", 6, column, readlen);
    ASSERT_FALSE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("test \" c", 8, column, readlen);
    ASSERT_FALSE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("\"test d\"", 8, column, readlen);
    ASSERT_TRUE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    if (!windowsReturn) {
        ASSERT_MEMEQ("Multiple\rline\rcell", 18, column, readlen);
    } else {
        ASSERT_MEMEQ("Multiple\r\nline\r\ncell", 20, column, readlen);
    }
        
    ASSERT_FALSE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("test b", 6, column, readlen);
    ASSERT_FALSE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("CSV", 3, column, readlen);
    ASSERT_FALSE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("123", 3, column, readlen);
    ASSERT_TRUE(islinelast);

    ASSERT_TRUE(reader->eof());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_EQ(NULL, column);
        
    ASSERT_TRUE(reader->eof());
        
    delete reader;
}

TEST(CSV, readcsv) {
    CSVReader *reader = new CSVReader();
    const char* test_file_path = "TESTCSV.csv";
    ASSERT_EQ(true, reader->open_path(test_file_path));

    helper_test_readcsv(reader, false);
}

TEST(CSV, readcsv3) {
    CSVReader *reader = new CSVReader();
    const char* test_file_path = "TESTCSV3.csv";
    ASSERT_EQ(true, reader->open_path(test_file_path));

    helper_test_readcsv(reader, true);
}

TEST(CSV, largecsv) {
    CSVReader *reader = new CSVReader();
    const char* test_file_path = "LARGECSV.csv";
    ASSERT_EQ(true, reader->open_path(test_file_path));

    bool islinelast;
    size_t readlen;
    const char *column;

    for (int i = 0; i < 1100; i++) {
        column = reader->readnext(&readlen, &islinelast);
        ASSERT_MEMEQ("Column A", 8, column, readlen);
        ASSERT_EQ(false, islinelast) << "Line" << i;

        column = reader->readnext(&readlen, &islinelast);
        ASSERT_MEMEQ("Column B\nMulti line!\n", 21, column, readlen);
        ASSERT_EQ(false, islinelast);
        ASSERT_FALSE(reader->eof());

        column = reader->readnext(&readlen, &islinelast);
        ASSERT_MEMEQ("Column C", 8, column, readlen);
        ASSERT_EQ(true, islinelast);
    }
    ASSERT_TRUE(reader->eof());

    column = reader->readnext(&readlen, &islinelast);
    if (column != NULL) {
        char *str = strndup(column, readlen);
        printf(":: %zu %s\n", readlen, str);
    }

    ASSERT_EQ(NULL, column);

    ASSERT_TRUE(reader->eof());
}

TEST(CSV, largecsv2) {
    CSVReader *reader = new CSVReader();
    const char* test_file_path = "LARGECSV2.csv";
    ASSERT_EQ(true, reader->open_path(test_file_path));

    bool islinelast;
    size_t readlen;
    const char *column;

    for (int i = 0; i < 1000; i++) {
        column = reader->readnext(&readlen, &islinelast);
        ASSERT_MEMEQ("Column \" A", 10, column, readlen);
        ASSERT_EQ(false, islinelast);
        ASSERT_FALSE(reader->eof());

        column = reader->readnext(&readlen, &islinelast);
        ASSERT_MEMEQ("Multi line \"\"\rcolumn", 20, column, readlen);
        ASSERT_EQ(true, islinelast);
    }
    ASSERT_TRUE(reader->eof());
        
    column = reader->readnext(&readlen, &islinelast);
    ASSERT_EQ(NULL, column);

    ASSERT_TRUE(reader->eof());
}

TEST(CSV,largecsv3) {
    CSVReader *reader = new CSVReader();
    const char* test_file_path = "LARGECSV3.csv";
    ASSERT_EQ(true, reader->open_path(test_file_path));

    bool islinelast;
    size_t readlen;
    const char *column;

    for (int i = 0; i < 2000; i++) {
        column = reader->readnext(&readlen, &islinelast);
        ASSERT_MEMEQ("1234567", 7, column, readlen);
        ASSERT_EQ(false, islinelast);
        ASSERT_FALSE(reader->eof());

        column = reader->readnext(&readlen, &islinelast);
        ASSERT_MEMEQ("ABCDEFG", 7, column, readlen);
        ASSERT_EQ(false, islinelast);
        ASSERT_FALSE(reader->eof());

        column = reader->readnext(&readlen, &islinelast);
        ASSERT_MEMEQ("abcdefg", 7, column, readlen);
        ASSERT_EQ(true, islinelast);
    }
    ASSERT_TRUE(reader->eof());
        
    column = reader->readnext(&readlen, &islinelast);
    ASSERT_EQ(NULL, column);

    ASSERT_TRUE(reader->eof());
}

TEST(CSV, largecsv4) {
    CSVReader *reader = new CSVReader();
    const char* test_file_path = "LARGECSV4.csv";
    ASSERT_EQ(true, reader->open_path(test_file_path));

    bool islinelast;
    size_t readlen;
    const char *column;

    for (int i = 0; i < 1100; i++) {
        column = reader->readnext(&readlen, &islinelast);
        ASSERT_MEMEQ("Column A", 8, column, readlen);
        ASSERT_EQ(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        ASSERT_MEMEQ("Column B\r\nMulti line!\r\n", 23, column, readlen);
        ASSERT_EQ(false, islinelast);
        ASSERT_FALSE(reader->eof());

        column = reader->readnext(&readlen, &islinelast);
        ASSERT_MEMEQ("Column C", 8, column, readlen);
        ASSERT_EQ(true, islinelast);
    }
    ASSERT_TRUE(reader->eof());

    column = reader->readnext(&readlen, &islinelast);
    if (column != NULL) {
        char *str = strndup(column, readlen);
        printf(":: %zu %s\n", readlen, str);
    }

    ASSERT_EQ(NULL, column);

    ASSERT_TRUE(reader->eof());
}
