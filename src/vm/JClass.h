//
// Created by jglrxavpok on 02/01/19.
//

#ifndef TINYJVM_JCLASS_H
#define TINYJVM_JCLASS_H


#include <tinyutf8/tinyutf8.h>
#include <map>
#include "bytecode/ConstantPool.h"
#include "JField.h"
#include "JMethod.h"

typedef std::map<utf8_string, JMethod*> MethodMap;
typedef std::map<utf8_string, JField*> FieldMap;

class JClass {

public:
    JClass(uint16_t accessFlag, const utf8_string& name, const utf8_string& supername, FieldMap& fieldMap, MethodMap& methodMap, CPInfo** constantPool);
    uint16_t accessFlags;
    const utf8_string& name;
    utf8_string* canonicalName;
    const utf8_string& superclass;
    uint16_t interfaceCount;
    utf8_string* interfaces;
    FieldMap& fieldMap;
    MethodMap& methodMap;
    CPInfo** constantPool;

    static void *operator new(size_t size) {
        return sdmalloc(size);
    }
    static void operator delete(void *block) {
        free(block);
    }
};


#endif //TINYJVM_JCLASS_H
