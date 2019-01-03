//
// Created by jglrxavpok on 02/01/19.
//

#ifndef TINYJVM_DESCRIPTOR_H
#define TINYJVM_DESCRIPTOR_H


#include <cstdint>
#include <tinyutf8/tinyutf8.h>

enum NativeType {
    VOID,
    INTEGER,
    FLOAT,
    DOUBLE,
    LONG,
    BOOLEAN,
    SHORT,
    BYTE,
    CHAR,

    INVALID = -1
} typedef NativeType;

struct BasicDescriptor {
    bool isNative;
    bool isArray;
    union {
        NativeType nativeType;
        utf8_string* className;
        const BasicDescriptor* elementType;
    };
    size_t descLength;
} typedef BasicDescriptor;

struct MethodDescriptor {
    const BasicDescriptor* returnType;
    uint32_t argCount;
    const BasicDescriptor** argumentTypes;
} typedef MethodDescriptor;

#endif //TINYJVM_DESCRIPTOR_H
