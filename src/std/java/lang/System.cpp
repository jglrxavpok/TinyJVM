//
// Created by jglrxavpok on 03/01/19.
//

#include "System.h"

static utf8_string _name = u8"java/lang/System";
static utf8_string _supername = u8"java/lang/Object";
static FieldMap _fieldMap;
static MethodMap _methodMap;

static CONSTANT_Utf8_info _name_info = MAKE_UTF8("java/lang/System");
static CONSTANT_Utf8_info _supername_info = MAKE_UTF8("java/lang/Object");
static CONSTANT_Class_info _class_info = {CONSTANT_Class, 1}; // constant pool starts at 1
static CONSTANT_Class_info _superclass_info = {CONSTANT_Class, 2};
static CPInfo* _constantPool[] = {
        (CPInfo*)&_name_info,
        (CPInfo*)&_supername_info,
        (CPInfo*)&_class_info,
        (CPInfo*)&_superclass_info,
};

_System::_System(): NativeClass(ACC_PUBLIC, _name, _supername, _fieldMap, _methodMap, _constantPool) {
    ;
}
