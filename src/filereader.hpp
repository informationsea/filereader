#ifndef _FILEREADER_H_
#define _FILEREADER_H_

#include "filereader_core.hpp"
#include "filereader_stdio.hpp"
#include "filereader_mmap.hpp"
#include "filereader_gzip.hpp"

class FileReader;

FileReader *filereader_get_suitable_reader(int fd);
FileReader *filereader_get_suitable_reader(const char* path);

#endif /* _FILEREADER_H_ */
