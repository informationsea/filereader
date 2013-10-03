#include <stdio.h>
#include <stdlib.h>

#include "filereader.hpp"

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "%s FILENAME start_position length\n", argv[0]);
        return 1;
    }

    char *endptr;
    long start_positon;
    long length;

    start_positon = strtol(argv[2], &endptr, 10);
    if (endptr != NULL && *endptr != '\0') {
        fprintf(stderr, "start_position should be integer\n");
        return 1;
    }

    length = strtol(argv[3], &endptr, 10);
    if (endptr != NULL && *endptr != '\0') {
        fprintf(stderr, "length should be integer\n");
        return 1;
    }

    StdioFileReader *reader = new StdioFileReader();
    reader->open_path(argv[1]);

    reader->seek(start_positon);
    size_t readlen;
    const char* buffer = reader->read(length, &readlen);
    for (size_t i = 0; i < readlen; i++)
        fputc(buffer[i], stdout);
 
    return 0;
}
