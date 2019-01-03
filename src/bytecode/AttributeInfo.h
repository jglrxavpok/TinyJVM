//
// Created by jglrxavpok on 02/01/19.
//

#ifndef TINYJVM_ATTRIBUTEINFO_H
#define TINYJVM_ATTRIBUTEINFO_H

#include "Arduino.h"
#include "sdmalloc.h"
#include <Stream.h>
#include <stdint.h>

class AttributeInfo {
public:
    uint16_t nameIndex;
    uint32_t length;
    uint8_t* info;
    static AttributeInfo* read(uint16_t count, Stream* s);

    static void *operator new(size_t size) {
        return sdmalloc(size);
    }
    static void operator delete(void *block) {
        free(block);
    }
private:
    static void readEntry(AttributeInfo* info, Stream* s);
};


#endif //TINYJVM_ATTRIBUTEINFO_H
