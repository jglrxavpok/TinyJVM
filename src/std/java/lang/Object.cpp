//
// Created by jglrxavpok on 03/01/19.
//

#include "Object.h"

static utf8_string _name = u8"java/lang/Object";
static utf8_string _supername = u8"<null>";
static FieldMap _fieldMap;
static MethodMap _methodMap;

static CONSTANT_Utf8_info _name_info = MAKE_UTF8("java/lang/Object");
static CONSTANT_Utf8_info _supername_info = MAKE_UTF8("<null>");
static CONSTANT_Class_info _class_info = {CONSTANT_Class, 1}; // constant pool starts at 1
static CONSTANT_Class_info _superclass_info = {CONSTANT_Class, 2};
static CPInfo* _constantPool[] = {
        (CPInfo*)&_name_info,
        (CPInfo*)&_supername_info,
        (CPInfo*)&_class_info,
        (CPInfo*)&_superclass_info,
};

_Object::_Object(): NativeClass(ACC_PUBLIC, _name, _supername, _fieldMap, _methodMap, _constantPool) {
    ;
}

NATIVE_CALLBACK_IMPL(_Object, hashCode) {
    return jvm.intValue((uint32_t)args[0].data);
}

NATIVE_CALLBACK_IMPL(_Object, toString) {
    size_t typeLength = args[0].type->className->length();
    char str[typeLength + 1 + 7 +1 /*null char*/];
    strcpy(str, args[0].type->className->data());
    str[typeLength] = '@';
    auto hashcode = (uint32_t)args[0].data;
    strcpy(str, itoa((int)hashcode, &str[typeLength+1], 10));
    return jvm.stringValue(new utf8_string(str));
}