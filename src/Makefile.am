AM_LDFLAGS = -no-undefined

bin_PROGRAMS = readfile
readfile_SOURCES = readfile.cpp
readfile_LDADD = libfilereader.la

lib_LTLIBRARIES = libfilereader.la
libfilereader_la_SOURCES = \
	filereader.cpp filereader_core.cpp filereader_stdio.cpp \
	filereader_mmap.cpp filereader_gzip.cpp tablereader.cpp csvreader.cpp
libfilereader_la_includedir = $(includedir)/filereader
libfilereader_la_include_HEADERS = \
	filereader.hpp filereader_stdio.hpp filereader_core.hpp \
	filereader_mmap.hpp filereader_gzip.hpp tablereader.hpp csvreader.hpp
libfilereader_la_LIBADD = $(zlib_LIBS)
