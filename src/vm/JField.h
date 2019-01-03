//
// Created by jglrxavpok on 02/01/19.
//

#ifndef TINYJVM_JFIELD_H
#define TINYJVM_JFIELD_H


#include <tinyutf8/tinyutf8.h>
#include <sdmalloc.h>
#include "Arduino.h"
#include "Descriptors.h"

class JField {
public:
    JField(const uint16_t accessFlag, const utf8_string& name, const BasicDescriptor& descriptor);
    const utf8_string& name;
    const BasicDescriptor& descriptor;
    const uint16_t accessFlags;

    static void *operator new(size_t size) {
        return sdmalloc(size);
    }
    static void operator delete(void *block) {
        free(block);
    }
private:

};


#endif //TINYJVM_JFIELD_H
