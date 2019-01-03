//
// Created by jglrxavpok on 02/01/19.
//

#include "ByteStream.h"
#include "Arduino.h"

ByteStream::ByteStream(uint8_t *array, size_t size): array(array), size(size), cursor(0) {
    Serial.println("hello");
}

size_t ByteStream::write(uint8_t b) {
    return 0;
}

size_t ByteStream::write(const uint8_t *buffer, size_t size) {
    return 0;
}

int ByteStream::availableForWrite(void) {
    return 0;
}

void ByteStream::flush() {
    ;
}

int ByteStream::available() {
    return size-cursor;
}

int ByteStream::read() {
    if(cursor >= size)
        return -1;
    return (unsigned char)array[cursor++];
}

int ByteStream::peek() {
    if(cursor >= size)
        return -1;
    return (unsigned char)array[cursor];
}
