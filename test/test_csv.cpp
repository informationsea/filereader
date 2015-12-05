#include <csvreader.hpp>
#include <stdio.h>
#include <string.h>
#include "test_common.hpp"

void helper_test_readcsv2(CSVReader *reader, const char *returnCode) {
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
    ASSERT_EQ(9+strlen(returnCode), reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("a", 1, column, readlen);
    ASSERT_FALSE(islinelast);
    ASSERT_EQ(11+strlen(returnCode), reader->tell());

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("pen", 3, column, readlen);
    ASSERT_TRUE(islinelast);
    ASSERT_EQ(15+strlen(returnCode), reader->tell());

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
    char buf[256];
    int len = snprintf(buf, sizeof(buf)-1, "Multiple%sline%scell", returnCode, returnCode);
    ASSERT_MEMEQ(buf, len, column, readlen);
        
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
        ASSERT_EQ(2, readlen) << "Line" << i;
        ASSERT_EQ(0, memcmp("12", column, 2)) << "Line " << i;
        ASSERT_EQ(false, islinelast) << "Line " << i;
        ASSERT_FALSE(reader->eof()) << "Line " << i;

        column = reader->readnext(&readlen, &islinelast);
        ASSERT_EQ(3, readlen) << "Line " << i << " " << column[0] << column[1] << column[2];
        ASSERT_EQ(0, memcmp(buf, column, 3)) << "Line " << i;
        ASSERT_EQ(true, islinelast) << "Line " << i;
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


TEST(CSV, largecsv_unix) {
    CSVReader *reader = new CSVReader();
    const char* test_file_path = "LARGECSV-unix.csv";
    ASSERT_EQ(true, reader->open_path(test_file_path));
    helper_largecsv(reader, "\n");
}

#if 0

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
        ASSERT_EQ(1, readlen) << "Line" << i;
        ASSERT_EQ(0, memcmp("1", column, 1)) << "Line " << i << " " << column[0];
        ASSERT_EQ(false, islinelast) << "Line " << i;
        ASSERT_FALSE(reader->eof()) << "Line " << i;

        column = reader->readnext(&readlen, &islinelast);
        ASSERT_EQ(3, readlen) << "Line " << i;
        ASSERT_EQ(0, memcmp("o\r\n", column, 3)) << "Line " << i;
        ASSERT_EQ(true, islinelast) << "Line " << i;
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

#endif
