#include <cppcutter.h>
#include <tablereader.hpp>

namespace table {

    void test_readtvs(void) {
        TableReader *reader = new TableReader();
        const char* test_file_path = cut_build_fixture_path(".", "TESTTABLE.TVS", NULL);
        cut_assert_equal_int(true, reader->open_path(test_file_path));

        bool islinelast;
        size_t readlen;
        const char *column;
        
        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("Column1", 7, column, readlen);
        cut_assert_equal_int(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("Column2", 7, column, readlen);
        cut_assert_equal_int(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("Column3", 7, column, readlen);
        cut_assert_equal_int(true, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("A", 1, column, readlen);
        cut_assert_equal_int(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("B", 1, column, readlen);
        cut_assert_equal_int(false, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert_equal_memory("C", 1, column, readlen);
        cut_assert_equal_int(true, islinelast);

        column = reader->readnext(&readlen, &islinelast);
        cut_assert(NULL == column);

        delete reader;
    }

}
