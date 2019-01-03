//
// Created by jglrxavpok on 03/01/19.
//

#ifndef TINYJVM_OBJECT_H
#define TINYJVM_OBJECT_H

#include "tinyutf8/tinyutf8.h"
#include <bytecode/Modifiers.h>
#include <bytecode/ConstantPool.h>
#include <std/NativeClass.h>
#include "vm/JClass.h"

class _Object: public NativeClass {
public:
    _Object();

    NATIVE_CALLBACK(toString)
    NATIVE_CALLBACK(hashCode)
};

static JClass* Object = (JClass*)new _Object;

#endif //TINYJVM_OBJECT_H
