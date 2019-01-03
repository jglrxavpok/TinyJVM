//
// Created by jglrxavpok on 02/01/19.
//

#ifndef TINYJVM_BYTESTREAM_H
#define TINYJVM_BYTESTREAM_H


#include <Stream.h>
#include "sdmalloc.h"

class ByteStream: public Stream {
private:
    uint8_t* array;
    size_t size;
    size_t cursor;

public:
    ByteStream(uint8_t* array, size_t size);

    size_t write(uint8_t b) override;

    size_t write(const uint8_t *buffer, size_t size) override;

    int availableForWrite(void) override;

    void flush() override;

    int available() override;

    int read() override;

    int peek() override;

    static void *operator new(size_t size) {
        return sdmalloc(size);
    }
    static void operator delete(void *block) {
        free(block);
    }
};


#endif //TINYJVM_BYTESTREAM_H
