//
// Created by jglrxavpok on 02/01/19.
//

#include <bytecode/ConstantPool.h>
#include "JClass.h"

JClass::JClass(uint16_t accessFlag, const utf8_string& name, const utf8_string& supername, FieldMap& fieldMap, MethodMap& methodMap, CPInfo** constantPool):
    accessFlags(accessFlag), name(name), superclass(supername), fieldMap(fieldMap), methodMap(methodMap), constantPool(constantPool) {

}
