AM_LDFLAGS = -module -rpath $(libdir) -avoid-version -no-undefined
AM_CPPFLAGS = $(CUTTER_CFLAGS)
LIBS = $(CUTTER_LIBS) $(top_builddir)/src/libfilereader.la

noinst_LTLIBRARIES = test_stdiotest.la
test_stdiotest_la_SOURCES = test_stdiotest.cpp

TESTS = run-test.sh
TESTS_ENVIRONMENT = CUTTER="$(CUTTER)"
