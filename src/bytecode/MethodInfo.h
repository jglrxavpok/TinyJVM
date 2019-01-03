//
// Created by jglrxavpok on 02/01/19.
//

#ifndef TINYJVM_METHODINFO_H
#define TINYJVM_METHODINFO_H


#include <Stream.h>
#include <stdint.h>
#include <sdmalloc.h>
#include "AttributeInfo.h"

class MethodInfo {
public:
    uint16_t accessFlags;
    uint16_t nameIndex;
    uint16_t descriptorIndex;
    uint16_t attributeCount;
    AttributeInfo* attributes;

    static MethodInfo* read(uint16_t count, Stream* s);

    static void *operator new(size_t size) {
        return sdmalloc(size);
    }
    static void operator delete(void *block) {
        free(block);
    }

private:
    static void readEntry(MethodInfo* entry, Stream* s);

};


#endif //TINYJVM_METHODINFO_H
