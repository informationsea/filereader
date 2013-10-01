#include <cppcutter.h>
#include <csvreader.hpp>

namespace csv {

    void test_readcsv(void) {
        CSVReader *reader = new CSVReader();
        const char* test_file_path = cut_build_fixture_path(".", "TESTCSV.csv", NULL);
        cut_assert_equal_int(true, reader->open_path(test_file_path));

        bool islinelast;
        size_t readlen;
        const char *column;
        
        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("A", 1, column, readlen);
        cut_assert_equal_int(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("B", 1, column, readlen);
        cut_assert_equal_int(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("C", 1, column, readlen);
        cut_assert_equal_int(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("D", 1, column, readlen);
        cut_assert_equal_int(true, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("This is a pen.", 14, column, readlen);
        cut_assert_equal_int(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("test b", 6, column, readlen);
        cut_assert_equal_int(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("test \" c", 8, column, readlen);
        cut_assert_equal_int(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("\"test d\"", 8, column, readlen);
        cut_assert_equal_int(true, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("Multiple\rline\rcell", 18, column, readlen);
        cut_assert_equal_int(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("test b", 6, column, readlen);
        cut_assert_equal_int(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("CSV", 3, column, readlen);
        cut_assert_equal_int(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("123", 3, column, readlen);
        cut_assert_equal_int(true, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_pointer(NULL, column);
        

        delete reader;
    }

    void test_largecsv(void) {
        CSVReader *reader = new CSVReader();
        const char* test_file_path = cut_build_fixture_path(".", "LARGECSV.csv", NULL);
        cut_assert_equal_int(true, reader->open_path(test_file_path));

        bool islinelast;
        size_t readlen;
        const char *column;

        for (int i = 1; i < 1100; i++) {
            column = reader->readnext(&readlen, &islinelast);
            cut_assert_equal_memory("Column A", 8, column, readlen, cut_message("Line %d", i));
            cut_assert_equal_int(false, islinelast, cut_message("Line %d", i));

            column = reader->readnext(&readlen, &islinelast);
            cut_assert_equal_memory("Column B\nMulti line!\n", 21, column, readlen, cut_message("Line %d", i));
            cut_assert_equal_int(false, islinelast, cut_message("Line %d", i));

            column = reader->readnext(&readlen, &islinelast);
            cut_assert_equal_memory("Column C", 8, column, readlen, cut_message("Line %d", i));
            cut_assert_equal_int(true, islinelast, cut_message("Line %d", i));
        }
    }
    
}
