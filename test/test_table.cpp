#include <tablereader.hpp>
#include "test_common.hpp"

TEST(TABLE, test_readtvs) {
    TableReader *reader = new TableReader();
    const char* test_file_path = "TESTTABLE.TVS";
    ASSERT_TRUE(reader->open_path(test_file_path));

    bool islinelast;
    size_t readlen;
    const char *column;
        
    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("Column1", 7, column, readlen);
    ASSERT_FALSE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("Column2", 7, column, readlen);
    ASSERT_FALSE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("Column3", 7, column, readlen);
    ASSERT_TRUE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("A", 1, column, readlen);
    ASSERT_FALSE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("B", 1, column, readlen);
    ASSERT_FALSE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("C", 1, column, readlen);
    ASSERT_TRUE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_EQ(NULL, column);

    delete reader;
}
    
TEST(TABLE,readtvs2) {
    TableReader *reader = new TableReader();
    const char* test_file_path = "TESTTABLE2.TVS";
    ASSERT_TRUE(reader->open_path(test_file_path));

    bool islinelast;
    size_t readlen;
    const char *column;
        
    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("Column1", 7, column, readlen);
    ASSERT_FALSE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("Column2", 7, column, readlen);
    ASSERT_FALSE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("Column3", 7, column, readlen);
    ASSERT_TRUE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("A", 1, column, readlen);
    ASSERT_FALSE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("B", 1, column, readlen);
    ASSERT_FALSE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_MEMEQ("C", 1, column, readlen);
    ASSERT_TRUE(islinelast);

    column = reader->readnext(&readlen, &islinelast);
    ASSERT_EQ(NULL, column);

    delete reader;
}
