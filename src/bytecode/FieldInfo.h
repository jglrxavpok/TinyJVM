//
// Created by jglrxavpok on 02/01/19.
//

#ifndef TINYJVM_FIELDINFO_H
#define TINYJVM_FIELDINFO_H


#include <stdint.h>
#include <Stream.h>
#include "AttributeInfo.h"

class FieldInfo {
public:
    uint16_t accessFlags;
    uint16_t nameIndex;
    uint16_t descriptorIndex;
    uint16_t attributeCount;
    AttributeInfo* attributes;

    static void *operator new(size_t size) {
        return sdmalloc(size);
    }
    static void operator delete(void *block) {
        free(block);
    }

    static FieldInfo* read(uint16_t count, Stream* s);

private:
    static void readEntry(FieldInfo* entry, Stream* s);
};


#endif //TINYJVM_FIELDINFO_H
