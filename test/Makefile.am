SUBDIRS = gtest

TESTS = $(noinst_PROGRAM)
EXTRA_DIST = run-test.sh TESTFILE TESTTABLE.TVS TESTCSV.csv TESTCSV2.csv LARGECSV.csv

LIBS += $(top_builddir)/src/libfilereader.la $(top_builddir)/test/gtest/libgtest.la

noinst_PROGRAMS = test_filereader test_table test_csv
test_filereader_SOURCES = test_filereader.cpp
test_table_SOURCES = test_table.cpp
test_csv_SOURCES = test_csv.cpp
