//
// Created by jglrxavpok on 03/01/19.
//

#include "NativeClass.h"

NativeClass::NativeClass(uint16_t accessFlag, const utf8_string &name, const utf8_string &supername, FieldMap &fieldMap,
                         MethodMap &methodMap, CPInfo **constantPool): JClass(accessFlag, name, supername, fieldMap, methodMap, constantPool) {

}
