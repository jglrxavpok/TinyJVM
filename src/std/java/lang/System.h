//
// Created by jglrxavpok on 03/01/19.
//

#ifndef TINYJVM_SYSTEM_H
#define TINYJVM_SYSTEM_H

#include "tinyutf8/tinyutf8.h"
#include <bytecode/Modifiers.h>
#include <bytecode/ConstantPool.h>
#include <std/NativeClass.h>
#include "vm/JClass.h"

class _System: public NativeClass {
public:
    _System();
};

static JClass* System = (JClass*)new _System;
#endif //TINYJVM_SYSTEM_H
