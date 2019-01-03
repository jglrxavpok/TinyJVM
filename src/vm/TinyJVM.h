//
// Created by jglrxavpok on 01/01/19.
//

#ifndef TINYJVM_TINYJVM_H
#define TINYJVM_TINYJVM_H

#include <map>
#include <stack>
#include <vector>
#include "bytecode/ClassFile.h"
#include "tinyutf8/tinyutf8.h"
#include "JClass.h"

struct Value {
    const BasicDescriptor* type;
    void* data;
} typedef Value;

struct CodeLocation {
    JMethod* currentMethod;
    JClass* currentClass;
    size_t programCounter;
} typedef CodeLocation;

typedef std::vector<Value> LocalStack;

template <typename T>
struct Pool {
    std::vector<T*> values;

    T* get() {
        if(values.empty())
            return (T*)sdmalloc(sizeof(T));
        T* value = values.back();
        values.pop_back();
        return value;
    }

    void store(T* value) {
        values.push_back(value);
    }
};

class TinyJVM {

private:
    Pool<CodeLocation> freeLocationStructs;
    Pool<Value> freeValueStructs;
    Pool<LocalStack> freeLocalStacks;
    CodeLocation currentLocation;
    std::map<utf8_string, JClass*> loadedClasses;
    std::map<utf8_string, const BasicDescriptor*> basicDescriptors;
    std::map<utf8_string, const MethodDescriptor*> methodDescriptors;
    std::stack<CodeLocation> callstack;
    std::stack<Value> executionStack;
    LocalStack locals;
    std::stack<LocalStack> localStack;
    const BasicDescriptor* parseBasicDescriptor(const utf8_string& str, size_t* readLength = nullptr);
    const MethodDescriptor* parseMethodDescriptor(const utf8_string& str);
    const BasicDescriptor *actualParseBasicDescriptor(const utf8_string &utf8_string, size_t* readLength = nullptr);


public:
    TinyJVM();
    void loadClassFile(ClassFile &file);
    void loadClass(JClass* clazz);
    void startFrom(utf8_string className, utf8_string mainMethodName); // TODO: main arguments

    void step();

    void buildFieldMap(ClassFile& file, FieldMap &map);
    void buildMethodMap(ClassFile& file, MethodMap &map);

    // Helper methods
    Value* stringValue(utf8_string* str);
    Value* intValue(uint32_t bytes);
    Value* floatValue(uint32_t bytes);

    static void *operator new(size_t size) {
        return sdmalloc(size);
    }
    static void operator delete(void *block) {
        free(block);
    }
};

typedef Value* (NativeCallback) (TinyJVM& jvm, size_t argCount, Value* args);

#endif //TINYJVM_TINYJVM_H
