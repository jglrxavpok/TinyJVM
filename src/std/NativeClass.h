//
// Created by jglrxavpok on 03/01/19.
//

#ifndef TINYJVM_NATIVECLASS_H
#define TINYJVM_NATIVECLASS_H


#include <vm/JClass.h>
#include <vm/TinyJVM.h>
#include <bytecode/ConstantPool.h>

#define NATIVE_CALLBACK(name) Value* name(TinyJVM& jvm, size_t argCount, Value* args);
#define NATIVE_CALLBACK_IMPL(clazz, name) Value* clazz::name(TinyJVM& jvm, size_t argCount, Value* args)
#define MAKE_UTF8(str) {CONSTANT_Utf8, (u2)sizeof(str), (u1*)str}

class NativeClass: public JClass {
public:
    NativeClass(uint16_t accessFlag, const utf8_string& name, const utf8_string& supername, FieldMap& fieldMap, MethodMap& methodMap, CPInfo** constantPool);
};


#endif //TINYJVM_NATIVECLASS_H
