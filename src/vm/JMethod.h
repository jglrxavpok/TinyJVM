//
// Created by jglrxavpok on 02/01/19.
//

#ifndef TINYJVM_JMETHOD_H
#define TINYJVM_JMETHOD_H


#include <tinyutf8/tinyutf8.h>
#include "CodeAttribute.h"
#include "Descriptors.h"

class JMethod {
public:
    JMethod(const uint16_t accessFlag, const utf8_string& name, const MethodDescriptor& descriptor, CodeAttribute* codeAttribute);
    const utf8_string& name;
    const MethodDescriptor& descriptor;
    CodeAttribute* codeAttribute;
    const uint16_t accessFlags;
    void* callback = nullptr;

    static void *operator new(size_t size) {
        return sdmalloc(size);
    }
    static void operator delete(void *block) {
        free(block);
    }
};


#endif //TINYJVM_JMETHOD_H
