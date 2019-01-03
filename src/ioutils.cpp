//
// Created by jglrxavpok on 01/01/19.
//

#include "ioutils.h"
#include "utils.h"

uint16_t ReadU2(Stream* s) {
    uint8_t buf[2];
    s->readBytes(buf, 2);
    return (buf[0] << 8) | (buf[1]);
}

uint32_t ReadU4(Stream* s) {
    uint8_t buf[4];
    s->readBytes(buf, 4);
    return (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | (buf[3]);
}

void ReadU2Array(Stream* s, uint16_t* array, size_t count) {
    // TODO: optimise via a buffer?
    for(size_t index = 0; index < count; index++)
        array[index] = ReadU2(s);
}

void ReadU4Array(Stream* s, uint32_t* array, size_t count) {
    // TODO: optimise via a buffer?
    for(size_t index = 0; index < count; index++)
        array[index] = ReadU4(s);
}

