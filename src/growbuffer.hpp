#ifndef GROWBUFFER_H
#define GROWBUFFER_H

#include <stdlib.h>
#include <string.h>

const size_t GROWBUFFER_DEFAULT_SIZE = 1024;

class GrowBuffer
{
public:
    GrowBuffer() : bufferSize(0), current(0) {
        buffer = (char *)malloc(GROWBUFFER_DEFAULT_SIZE);
        if (buffer) {
            bufferSize = GROWBUFFER_DEFAULT_SIZE;
        }
    }
    
    virtual ~GrowBuffer() {
        free(buffer);
    }

    bool append(char value) {
        if (current >= bufferSize) {
            char *newbuffer = (char *)realloc(buffer, current + GROWBUFFER_DEFAULT_SIZE);
            if (newbuffer == NULL)
                return false;
            buffer = newbuffer;
            bufferSize += GROWBUFFER_DEFAULT_SIZE;
        }
        buffer[current++] = value;
        return true;
    }

    bool append(const char* value, size_t len) {
        if (current + len >= bufferSize) {
            char *newbuffer = (char *)realloc(buffer, (((current + len)/GROWBUFFER_DEFAULT_SIZE) + 1)*GROWBUFFER_DEFAULT_SIZE);
            if (newbuffer == NULL)
                return false;
            buffer = newbuffer;
            bufferSize += GROWBUFFER_DEFAULT_SIZE;
        }
        memcpy(buffer + current, value, len);
        current += len;
        return true;
    }

    const char *data() const {
        return buffer;
    }

    char operator[](size_t i) const {
        return buffer[i];
    }

    size_t size() const {
        return current;
    }

    size_t reservedSize() const {
        return bufferSize;
    }

    bool normalizeQuote() {
        int diff = 0;

        for (size_t src = 1; src < current; src++) {
            if (buffer[src] == '"' && buffer[src-1] == '"') { // TODO: optimize here
                diff += 1;
            } else {
                buffer[src - diff] = buffer[src];
            }
        }
        current -= diff;
        return true;
    }

    void clear() {
        current = 0;
    }

    bool cleanAndShrink() {
        if (bufferSize != GROWBUFFER_DEFAULT_SIZE) {
            char *newbuffer = (char *)malloc(GROWBUFFER_DEFAULT_SIZE);
            if (newbuffer == NULL)
                return false;
            buffer = newbuffer;
        }
        current = 0;
        return true;
    }

private:
    char *buffer;
    size_t bufferSize;
    size_t current;
};

#endif /* GROWBUFFER_H */
