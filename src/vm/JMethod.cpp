//
// Created by jglrxavpok on 02/01/19.
//

#include "JMethod.h"

JMethod::JMethod(const uint16_t accessFlag, const utf8_string &name, const MethodDescriptor &descriptor, CodeAttribute* codeAttribute): accessFlags(accessFlag), name(name), descriptor(descriptor), codeAttribute(codeAttribute) {

}
