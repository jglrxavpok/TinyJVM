//
// Created by jglrxavpok on 01/01/19.
//

#ifndef TINYJVM_CLASSFILE_H
#define TINYJVM_CLASSFILE_H

#include <stdint.h>
#include <Stream.h>
#include "ConstantPool.h"
#include "FieldInfo.h"
#include "MethodInfo.h"

class ClassFile {
public:
    ClassFile(uint32_t magicNumber, uint16_t minor, uint16_t major, uint16_t constantPoolCount, CPInfo** constantPool,
            uint16_t accessFlags, uint16_t thisCLass, uint16_t superClass, uint16_t interfaceCount, uint16_t* interfaces,
            uint16_t fieldCount, FieldInfo* fields, uint16_t methodCount, MethodInfo* methods,
            uint16_t attributeCount, AttributeInfo* attributes);

    uint32_t magicNumber;
    uint16_t minorVersion;
    uint16_t majorVersion;
    uint16_t constantPoolCount;
    CPInfo** constantPool;
    uint16_t accessFlags;
    uint16_t thisClass;
    uint16_t superClass;
    uint16_t interfaceCount;
    uint16_t* interfaces;
    uint16_t fieldCount;
    FieldInfo* fields;
    uint16_t methodCount;
    MethodInfo* methods;
    uint16_t attributeCount;
    AttributeInfo* attributes;

    static void *operator new(size_t size) {
        return sdmalloc(size);
    }
    static void operator delete(void *block) {
        free(block);
    }

    static ClassFile* read(Stream* s);
};


#endif //TINYJVM_CLASSFILE_H
