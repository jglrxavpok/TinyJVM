//
// Created by jglrxavpok on 01/01/19.
//

#ifndef TINYJVM_TINYJVM_H
#define TINYJVM_TINYJVM_H

#include <map>
#include <stack>
#include "tinyutf8/tinyutf8.h"
#include "JClass.h"

struct Value {
    const BasicDescriptor* type;
    void* data;
};

struct CodeLocation {
    JMethod* currentMethod;
    JClass* currentClass;
    size_t programCounter;
};

class TinyJVM {

private:
    CodeLocation currentLocation;
    std::map<utf8_string, JClass*> loadedClasses;
    std::map<utf8_string, const BasicDescriptor*> basicDescriptors;
    std::map<utf8_string, const MethodDescriptor*> methodDescriptors;
    std::stack<CodeLocation> callstack;
    std::stack<Value> executionStack;
    const BasicDescriptor* parseBasicDescriptor(const utf8_string& str, size_t* readLength = nullptr);
    const MethodDescriptor* parseMethodDescriptor(const utf8_string& str);
    const BasicDescriptor *actualParseBasicDescriptor(const utf8_string &utf8_string, size_t* readLength = nullptr);


public:
    TinyJVM();
    void loadClassFile(ClassFile &file);

    void startFrom(utf8_string className, utf8_string mainMethodName); // TODO: main arguments

    void step();

    void buildFieldMap(ClassFile& file, FieldMap &map);
    void buildMethodMap(ClassFile& file, MethodMap &map);

    static void *operator new(size_t size) {
        return sdmalloc(size);
    }
    static void operator delete(void *block) {
        free(block);
    }
};


#endif //TINYJVM_TINYJVM_H
