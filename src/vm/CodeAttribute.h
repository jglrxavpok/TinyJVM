//
// Created by jglrxavpok on 02/01/19.
//

#ifndef TINYJVM_CODEATTRIBUTE_H
#define TINYJVM_CODEATTRIBUTE_H

#include "bytecode/class_format_types.h"
#include <stdint.h>
#include <bytecode/AttributeInfo.h>
#include <sdmalloc.h>

class CodeAttribute {
public:
    explicit CodeAttribute(AttributeInfo& info);

    u2 getMaxStack();
    u2 getMaxLocals();
    u4 getCodeLength();
    u1* getCode();

    static void *operator new(size_t size) {
        return sdmalloc(size);
    }
    static void operator delete(void *block) {
        free(block);
    }

private:
    u2 maxStack;
    u2 maxLocals;
    u4 codeLength;
    u1* code;

};


#endif //TINYJVM_CODEATTRIBUTE_H
