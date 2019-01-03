//
// Created by jglrxavpok on 01/01/19.
//

#ifndef TINYJVM_CONSTANTPOOL_H
#define TINYJVM_CONSTANTPOOL_H

#include "sdmalloc.h"
#include "class_format_types.h"
#include <Stream.h>
#include <tinyutf8/tinyutf8.h>

// Tag types
enum TagTypes {
    CONSTANT_Class = 7,
    CONSTANT_Fieldref = 9,
    CONSTANT_Methodref = 10,
    CONSTANT_InterfaceMethodref = 11,
    CONSTANT_String = 8,
    CONSTANT_Integer = 3,
    CONSTANT_Float = 4,
    CONSTANT_Long = 5,
    CONSTANT_Double = 6,
    CONSTANT_NameAndType = 12,
    CONSTANT_Utf8 = 1,
    CONSTANT_MethodHandle = 15,
    CONSTANT_MethodType = 16,
    CONSTANT_InvokeDynamic = 18
} typedef TagTypes;

struct CPInfo {
    u1 tag;
    u1 rawInfo[];
} typedef CPInfo;

struct CONSTANT_Class_info {
    u1 tag;
    u2 name_index;
} typedef CONSTANT_Class_info;

struct CONSTANT_Fieldref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
} typedef CONSTANT_Fieldref_info;

struct CONSTANT_Methodref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
} typedef CONSTANT_Methodref_info;

struct CONSTANT_InterfaceMethodref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
} typedef CONSTANT_InterfaceMethodref_info;

struct CONSTANT_String_info {
    u1 tag;
    u2 string_index;
} typedef CONSTANT_String_info;

struct CONSTANT_Integer_info {
    u1 tag;
    u4 bytes;
} typedef CONSTANT_Integer_info;

struct CONSTANT_Float_info {
    u1 tag;
    u4 bytes;
} typedef CONSTANT_Float_info;

struct CONSTANT_Long_info {
    u1 tag;
    u4 high_bytes;
    u4 low_bytes;
} typedef CONSTANT_Long_info;

struct CONSTANT_Double_info {
    u1 tag;
    u4 high_bytes;
    u4 low_bytes;
} typedef CONSTANT_Double_info;

struct CONSTANT_NameAndType_info {
    u1 tag;
    u2 name_index;
    u2 descriptor_index;
} typedef CONSTANT_NameAndType_info;

struct CONSTANT_Utf8_info {
    u1 tag;
    u2 length;
    u1* bytes;
} typedef CONSTANT_Utf8_info;

struct CONSTANT_MethodHandle_info {
    u1 tag;
    u1 reference_kind;
    u2 reference_index;
} typedef CONSTANT_MethodHandle_info;

struct CONSTANT_MethodType_info {
    u1 tag;
    u2 descriptor_index;
} typedef CONSTANT_MethodType_info;

struct CONSTANT_InvokeDynamic_info {
    u1 tag;
    u2 bootstrap_method_attr_index;
    u2 name_and_type_index;
} typedef CONSTANT_InvokeDynamic_info;

class ConstantPool {
public:
    static CPInfo** read(uint16_t count, Stream* s);
    static utf8_string * getUTF8(CPInfo** constants, uint16_t index);

private:
    ConstantPool() = default;
    static void readEntry(CPInfo**, Stream* s);
    static void read_CONSTANT_Class(Stream* stream, CONSTANT_Class_info* info);
    static void read_CONSTANT_Fieldref(Stream* stream, CONSTANT_Fieldref_info* info);
    static void read_CONSTANT_Methodref(Stream* stream, CONSTANT_Methodref_info* info);
    static void read_CONSTANT_InterfaceMethodref(Stream* stream, CONSTANT_InterfaceMethodref_info* info);
    static void read_CONSTANT_String(Stream* stream, CONSTANT_String_info* info);
    static void read_CONSTANT_Integer(Stream* stream, CONSTANT_Integer_info * pInfo);
    static void read_CONSTANT_Float(Stream* stream, CONSTANT_Float_info * pInfo);
    static void read_CONSTANT_Long(Stream* stream, CONSTANT_Long_info * pInfo);
    static void read_CONSTANT_Double(Stream* stream, CONSTANT_Double_info * pInfo);
    static void read_CONSTANT_NameAndType(Stream* stream, CONSTANT_NameAndType_info* pInfo);
    static void read_CONSTANT_MethodHandle(Stream* stream, CONSTANT_MethodHandle_info*pInfo);
    static void read_CONSTANT_MethodType(Stream* stream, CONSTANT_MethodType_info*  pInfo);
    static void read_CONSTANT_InvokeDynamic(Stream* stream, CONSTANT_InvokeDynamic_info* pInfo);
};

#define READ_CONSTANT_ENTRY(entry_type) {\
    auto* constant_info = (entry_type##_info *)(sdmalloc(sizeof(entry_type ## _info))); \
    read_##entry_type(s, constant_info); \
    *info = (CPInfo *)(constant_info); \
}

#endif //TINYJVM_CONSTANTPOOL_H
