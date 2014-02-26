if WITH_CUTTER
TESTS = run-test.sh
TESTS_ENVIRONMENT = CUTTER="$(CUTTER)"
EXTRA_DIST = run-test.sh TESTFILE TESTTABLE.TVS TESTCSV.csv TESTCSV2.csv LARGECSV.csv

AM_LDFLAGS = -module -rpath $(libdir) -avoid-version -no-undefined
AM_CPPFLAGS = $(CPPCUTTER_CFLAGS) -O0 -g
LIBS += $(CPPCUTTER_LIBS) $(top_builddir)/src/libfilereader.la

noinst_LTLIBRARIES = test_filereader.la test_table.la test_csv.la
test_filereader_la_SOURCES = test_filereader.cpp
test_table_la_SOURCES = test_table.cpp
test_csv_la_SOURCES = test_csv.cpp

echo-cutter:
	@echo $(CUTTER)

endif
