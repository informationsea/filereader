#include <csvreader.hpp>
#include <stdio.h>
#include <string.h>
#include "test_common.hpp"

void helper_test_readcsv2(CSVReader *reader, const char *returnCode) {
    bool islinelast;
    size_t readlen;
    const char *column;
        
    column = reader->readnext(&readlen, &islinelast);
    ASSERT_STREQ("This", column);
    ASSERT_FALSE(islinelast);
    ASSERT_EQ(5, reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_STREQ("", column);
    ASSERT_FALSE(islinelast);
    ASSERT_EQ(6, reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_STREQ("", column);
    ASSERT_TRUE(islinelast);
    ASSERT_EQ(7, reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_STREQ("is", column);
    ASSERT_FALSE(islinelast);
    ASSERT_EQ(9+strlen(returnCode), reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_STREQ("a", column);
    ASSERT_FALSE(islinelast);
    ASSERT_EQ(11+strlen(returnCode), reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_STREQ("pen", column);
    ASSERT_TRUE(islinelast);
    ASSERT_EQ(14+strlen(returnCode), reader->tell());

    ASSERT_TRUE(reader->eof());
}

TEST(CSV, readcsv2_unix) {
    CSVReader *reader = new CSVReader();
    const char* test_file_path = "TESTCSV2-unix.csv";
    ASSERT_TRUE(reader->open_path(test_file_path));
    helper_test_readcsv2(reader, "\n");
    delete reader;
}

TEST(CSV, readcsv2_dos) {
    CSVReader *reader = new CSVReader();
    const char* test_file_path = "TESTCSV2-dos.csv";
    ASSERT_TRUE(reader->open_path(test_file_path));
    helper_test_readcsv2(reader, "\r\n");
    delete reader;
}

TEST(CSV, readcsv2_mac) {
    CSVReader *reader = new CSVReader();
    const char* test_file_path = "TESTCSV2-mac.csv";
    ASSERT_TRUE(reader->open_path(test_file_path));
    helper_test_readcsv2(reader, "\r");
    delete reader;
}

void helper_test_readcsv(CSVReader *reader, const char *returnCode) {
    bool islinelast;
    size_t readlen;
    const char *column;
        
    column = reader->readnext(&readlen, &islinelast);
    ASSERT_STREQ("A", column);
    ASSERT_FALSE(islinelast);
    ASSERT_EQ(2, reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_STREQ("B", column);
    ASSERT_FALSE(islinelast);
    ASSERT_EQ(4, reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_STREQ("C", column);
    ASSERT_EQ(6, reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_STREQ("D", column);
    ASSERT_EQ(8, reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_STREQ("This is a pen.", column);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_STREQ("test b", column);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_STREQ("test \" c", column);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_STREQ("\"test d\"", column);

    column = reader->readnext(&readlen, &islinelast);
    char buf[256];
    int len = snprintf(buf, sizeof(buf)-1, "Multiple%sline%scell", returnCode, returnCode);
    ASSERT_STREQ(buf, column);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_STREQ("test b", column);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_STREQ("CSV", column);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_STREQ("123", column);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_EQ(NULL, column);
        
    ASSERT_TRUE(reader->eof());
        
    delete reader;
}

TEST(CSV, readcsv_mac) {
    CSVReader *reader = new CSVReader();
    const char* test_file_path = "TESTCSV-mac.csv";
    ASSERT_EQ(true, reader->open_path(test_file_path));

    helper_test_readcsv(reader, "\r");
}

TEST(CSV, readcsv_dos) {
    CSVReader *reader = new CSVReader();
    const char* test_file_path = "TESTCSV-dos.csv";
    ASSERT_EQ(true, reader->open_path(test_file_path));

    helper_test_readcsv(reader, "\r\n");
}

TEST(CSV, readcsv_unix) {
    CSVReader *reader = new CSVReader();
    const char* test_file_path = "TESTCSV-unix.csv";
    ASSERT_EQ(true, reader->open_path(test_file_path));

    helper_test_readcsv(reader, "\n");
}

void helper_largecsv(CSVReader *reader, const char* newline) {
    bool islinelast;
    size_t readlen;
    const char *column;

    char buf[30];
    snprintf(buf, sizeof(buf)-1, "o%sk", newline);
    
    for (int i = 0; i < 10250; i++) {
        //fprintf(stderr, "line %d\n", i);
        column = reader->readnext(&readlen, &islinelast);
        ASSERT_STREQ("12", column) << "Line " << i;
        ASSERT_EQ(false, islinelast) << "Line " << i;
        ASSERT_FALSE(reader->eof()) << "Line " << i;

        column = reader->readnext(&readlen, &islinelast);
        ASSERT_STREQ(buf, column) << "Line " << i;
        ASSERT_EQ(true, islinelast) << "Line " << i;
    }
    ASSERT_TRUE(reader->eof());

    column = reader->readnext(&readlen, &islinelast);
    if (column != NULL) {
        char *str = strndup(column, readlen);
        printf(":: %zu %s\n", readlen, str);
    }

    ASSERT_EQ(NULL, column) << column;

    ASSERT_TRUE(reader->eof());
}


TEST(CSV, largecsv_unix) {
    CSVReader *reader = new CSVReader();
    const char* test_file_path = "LARGECSV-unix.csv";
    ASSERT_EQ(true, reader->open_path(test_file_path));
    helper_largecsv(reader, "\n");
}

#if 2

TEST(CSV, largecsv_mac) {
    CSVReader *reader = new CSVReader();
    const char* test_file_path = "LARGECSV-mac.csv";
    ASSERT_EQ(true, reader->open_path(test_file_path));
    helper_largecsv(reader, "\r");
}


TEST(CSV, largecsv_dos) {
    CSVReader *reader = new CSVReader();
    const char* test_file_path = "LARGECSV-dos.csv";
    ASSERT_EQ(true, reader->open_path(test_file_path));

    bool islinelast;
    size_t readlen;
    const char *column;

    for (int i = 0; i < 10250; i++) {
        column = reader->readnext(&readlen, &islinelast);
        ASSERT_STREQ("1", column) << "Line " << i;
        ASSERT_EQ(false, islinelast) << "Line " << i;
        ASSERT_FALSE(reader->eof()) << "Line " << i;

        column = reader->readnext(&readlen, &islinelast);
        ASSERT_STREQ("o\r\n", column) << "Line " << i;
        ASSERT_EQ(true, islinelast) << "Line " << i;
    }

    column = reader->readnext(&readlen, &islinelast);
    if (column != NULL) {
        char *str = strndup(column, readlen);
        printf(":: %zu %s\n", readlen, str);
    }

    ASSERT_EQ(NULL, column);

    ASSERT_TRUE(reader->eof());
}

#endif
