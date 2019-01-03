//
// Created by jglrxavpok on 03/01/19.
//

#ifndef TINYJVM_SDMALLOC_H
#define TINYJVM_SDMALLOC_H

#include <stdint.h>
#include <string.h>

#define TEST_MEMORY \
size_t _consumed = 0; \
while(malloc(100)) _consumed+=100; \
Serial.printf("Failed to allocate after %i bytes (%i kb)!\n", _consumed, _consumed/1024);
//while(1);

void* sdmalloc(size_t size);

#endif //TINYJVM_SDMALLOC_H
