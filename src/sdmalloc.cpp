//
// Created by jglrxavpok on 03/01/19.
//

#include "Arduino.h"
#include "sdmalloc.h"
#include "utils.h"

static uint32_t consumed = 0;

void* sdmalloc(size_t size) {
    consumed += size;
    Serial.printf("[SDMalloc] Consumed %li bytes (%li kib)!\n", consumed, consumed/1024);
    void* result = malloc(size);
    while(!result) {
        Serial.printf("[SDMalloc] Failed to allocate %i, retrying\n", size);
        result = malloc(MIN(size, 100)); // reattempt
    }
    Serial.printf("[SDMalloc] Allocated %i at %p\n", size, result);
    return result;
}