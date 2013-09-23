AM_LDFLAGS = -no-undefined

bin_PROGRAMS = readfile
readfile_SOURCES = test.cpp
readfile_LDADD = libfilereader.la

lib_LTLIBRARIES = libfilereader.la
libfilereader_la_SOURCES = filereader.cpp tablereader.cpp
libfilereader_la_includedir = $(includedir)/filereader
libfilereader_la_include_HEADERS = filereader.hpp tablereader.hpp
