#include <cppcutter.h>
#include <csvreader.hpp>
#include <stdio.h>

namespace csv {

    void test_readcsv2(void) {
        CSVReader *reader = new CSVReader();
        const char* test_file_path = cut_build_fixture_path(".", "TESTCSV2.csv", NULL);
        cut_assert_equal_int(true, reader->open_path(test_file_path));

        bool islinelast;
        size_t readlen;
        const char *column;
        
        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("This", 4, column, readlen);
        cut_assert_equal_boolean(false, islinelast);
        cut_assert_equal_int(5, reader->tell());

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("", 0, column, readlen);
        cut_assert_equal_boolean(false, islinelast);
        cut_assert_equal_int(6, reader->tell());

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("", 0, column, readlen);
        cut_assert_equal_boolean(true, islinelast);
        cut_assert_equal_int(7, reader->tell());

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("is", 2, column, readlen);
        cut_assert_equal_boolean(false, islinelast);
        cut_assert_equal_int(10, reader->tell());

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("a", 1, column, readlen);
        cut_assert_equal_boolean(false, islinelast);
        cut_assert_equal_int(12, reader->tell());

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("pen", 3, column, readlen);
        cut_assert_equal_boolean(true, islinelast);
        cut_assert_equal_int(16, reader->tell());

        cut_assert_equal_boolean(true, reader->eof());
        
        delete reader;
    }

    void test_readcsv(void) {
        CSVReader *reader = new CSVReader();
        const char* test_file_path = cut_build_fixture_path(".", "TESTCSV.csv", NULL);
        cut_assert_equal_int(true, reader->open_path(test_file_path));

        bool islinelast;
        size_t readlen;
        const char *column;
        
        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("A", 1, column, readlen);
        cut_assert_equal_boolean(false, islinelast);
        cut_assert_equal_int(2, reader->tell());

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("B", 1, column, readlen);
        cut_assert_equal_boolean(false, islinelast);
        cut_assert_equal_int(4, reader->tell());

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("C", 1, column, readlen);
        cut_assert_equal_boolean(false, islinelast);
        cut_assert_equal_int(6, reader->tell());

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("D", 1, column, readlen);
        cut_assert_equal_boolean(true, islinelast);
        cut_assert_equal_int(8, reader->tell());

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("This is a pen.", 14, column, readlen);
        cut_assert_equal_boolean(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("test b", 6, column, readlen);
        cut_assert_equal_boolean(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("test \" c", 8, column, readlen);
        cut_assert_equal_boolean(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("\"test d\"", 8, column, readlen);
        cut_assert_equal_boolean(true, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("Multiple\rline\rcell", 18, column, readlen);
        cut_assert_equal_boolean(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("test b", 6, column, readlen);
        cut_assert_equal_boolean(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("CSV", 3, column, readlen);
        cut_assert_equal_boolean(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("123", 3, column, readlen);
        cut_assert_equal_boolean(true, islinelast);

        cut_assert_equal_boolean(true, reader->eof());

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_pointer(NULL, column);
        
        cut_assert_equal_boolean(true, reader->eof());
        
        delete reader;
    }

    void test_largecsv(void) {
        CSVReader *reader = new CSVReader();
        const char* test_file_path = cut_build_fixture_path(".", "LARGECSV.csv", NULL);
        cut_assert_equal_int(true, reader->open_path(test_file_path));

        bool islinelast;
        size_t readlen;
        const char *column;

        for (int i = 0; i < 1100; i++) {
            column = reader->readnext(&readlen, &islinelast);
            cut_assert_equal_memory("Column A", 8, column, readlen, cut_message("Line %d", i));
            cut_assert_equal_int(false, islinelast, cut_message("Line %d", i));

            column = reader->readnext(&readlen, &islinelast);
            cut_assert_equal_memory("Column B\nMulti line!\n", 21, column, readlen,
                                    cut_message("Line %d", i));
            cut_assert_equal_int(false, islinelast, cut_message("Line %d", i));
            cut_assert_equal_boolean(false, reader->eof());

            column = reader->readnext(&readlen, &islinelast);
            cut_assert_equal_memory("Column C", 8, column, readlen, cut_message("Line %d", i));
            cut_assert_equal_int(true, islinelast, cut_message("Line %d", i));
        }
        cut_assert_equal_boolean(true, reader->eof());

        column = reader->readnext(&readlen, &islinelast);
        if (column != NULL) {
            char *str = strndup(column, readlen);
            printf(":: %zu %s\n", readlen, str);
        }

        cut_assert_equal_pointer(NULL, column);

        cut_assert_equal_boolean(true, reader->eof());
    }

    void test_largecsv2(void) {
        CSVReader *reader = new CSVReader();
        const char* test_file_path = cut_build_fixture_path(".", "LARGECSV2.csv", NULL);
        cut_assert_equal_int(true, reader->open_path(test_file_path));

        bool islinelast;
        size_t readlen;
        const char *column;

        for (int i = 0; i < 1000; i++) {
            column = reader->readnext(&readlen, &islinelast);
            cut_assert_equal_memory("Column \" A", 10, column, readlen, cut_message("Line %d", i));
            cut_assert_equal_int(false, islinelast, cut_message("Line %d", i));
            cut_assert_equal_boolean(false, reader->eof());

            column = reader->readnext(&readlen, &islinelast);
            cut_assert_equal_memory("Multi line \"\"\rcolumn", 20, column, readlen, cut_message("Line %d", i));
            cut_assert_equal_int(true, islinelast, cut_message("Line %d", i));
        }
        cut_assert_equal_boolean(true, reader->eof());
        
        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_pointer(NULL, column);

        cut_assert_equal_boolean(true, reader->eof());
    }

    void test_largecsv3(void) {
        CSVReader *reader = new CSVReader();
        const char* test_file_path = cut_build_fixture_path(".", "LARGECSV3.csv", NULL);
        cut_assert_equal_int(true, reader->open_path(test_file_path));

        bool islinelast;
        size_t readlen;
        const char *column;

        for (int i = 0; i < 2000; i++) {
            column = reader->readnext(&readlen, &islinelast);
            cut_assert_equal_memory("1234567", 7, column, readlen, cut_message("Line %d", i));
            cut_assert_equal_int(false, islinelast, cut_message("Line %d", i));
            cut_assert_equal_boolean(false, reader->eof());

            column = reader->readnext(&readlen, &islinelast);
            cut_assert_equal_memory("ABCDEFG", 7, column, readlen, cut_message("Line %d", i));
            cut_assert_equal_int(false, islinelast, cut_message("Line %d", i));
            cut_assert_equal_boolean(false, reader->eof());

            column = reader->readnext(&readlen, &islinelast);
            cut_assert_equal_memory("abcdefg", 7, column, readlen, cut_message("Line %d", i));
            cut_assert_equal_int(true, islinelast, cut_message("Line %d", i));
        }
        cut_assert_equal_boolean(true, reader->eof());
        
        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_pointer(NULL, column);

        cut_assert_equal_boolean(true, reader->eof());
    }
}
