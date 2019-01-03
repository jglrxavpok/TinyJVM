//
// Created by jglrxavpok on 01/01/19.
//

#include "Arduino.h"
#include "ConstantPool.h"
#include "ioutils.h"
#include <unordered_map>
#include "sdmalloc.h"

static std::unordered_map<uint64_t, utf8_string*> utf8Cache;

utf8_string* ConstantPool::getUTF8(CPInfo** constants, uint16_t index) {
    CPInfo* info = constants[index-1];
    if(info->tag != CONSTANT_Utf8) {
        Serial.printf("Tag is not CONSTANT_Utf8 but %i\n", info->tag);
        // TODO: log
        return nullptr;
    }
    Serial.println("getUTF8");
    if(utf8Cache.find((uint64_t)info) != utf8Cache.end()) {
        Serial.println("cache hit in getUTF8");
        return utf8Cache.find((uint64_t)info)->second;
    }
    Serial.println("cache miss in getUTF8");
    auto * utf8_info = (CONSTANT_Utf8_info*)info;

    uint16_t length = utf8_info->length;
    const uint8_t* bytes = utf8_info->bytes;
    Serial.println("pre allocation of new utf8");
    auto* result = new utf8_string((char*)bytes, length);
    Serial.println("allocated new utf8");
    utf8Cache[(uint64_t)info] = result;
    return result;
}

void ConstantPool::read_CONSTANT_InvokeDynamic(Stream* stream, CONSTANT_InvokeDynamic_info*info) {
    info->bootstrap_method_attr_index = ReadU2(stream);
    info->name_and_type_index = ReadU2(stream);
}

void ConstantPool::read_CONSTANT_MethodType(Stream* stream, CONSTANT_MethodType_info*info) {
    info->descriptor_index = ReadU2(stream); // index
}

void ConstantPool::read_CONSTANT_MethodHandle(Stream *stream, CONSTANT_MethodHandle_info* info) {
    info->reference_kind = (u1)(stream->read());
    info->reference_index = ReadU2(stream);
}

void ConstantPool::read_CONSTANT_NameAndType(Stream* stream, CONSTANT_NameAndType_info* info) {
    info->name_index = ReadU2(stream);
    info->descriptor_index = ReadU2(stream);
}

void ConstantPool::read_CONSTANT_Double(Stream* stream, CONSTANT_Double_info* info) {
    info->high_bytes = ReadU4(stream);
    info->low_bytes = ReadU4(stream);
}

void ConstantPool::read_CONSTANT_Long(Stream* stream, CONSTANT_Long_info* info) {
    info->high_bytes = ReadU4(stream);
    info->low_bytes = ReadU4(stream);
}

void ConstantPool::read_CONSTANT_Float(Stream* stream, CONSTANT_Float_info* info) {
    info->bytes = ReadU4(stream); // bytes
}

void ConstantPool::read_CONSTANT_Integer(Stream* stream, CONSTANT_Integer_info* info) {
    info->bytes = ReadU4(stream); // bytes
}

void ConstantPool::read_CONSTANT_String(Stream* stream, CONSTANT_String_info* info) {
    info->string_index = ReadU2(stream); // index
}

void ConstantPool::read_CONSTANT_InterfaceMethodref(Stream* stream, CONSTANT_InterfaceMethodref_info* info) {
    info->class_index = ReadU2(stream);
    info->name_and_type_index = ReadU2(stream);
}

void ConstantPool::read_CONSTANT_Methodref(Stream* stream, CONSTANT_Methodref_info* info) {
    info->class_index = ReadU2(stream);
    info->name_and_type_index = ReadU2(stream);
}

void ConstantPool::read_CONSTANT_Fieldref(Stream* stream, CONSTANT_Fieldref_info* info) {
    info->class_index = ReadU2(stream);
    info->name_and_type_index = ReadU2(stream);
}

void ConstantPool::read_CONSTANT_Class(Stream* stream, CONSTANT_Class_info* info) {
    info->name_index = ReadU2(stream);
}

CPInfo** ConstantPool::read(uint16_t entryCount, Stream *s) {
    auto* array = (CPInfo**) sdmalloc(sizeof(CPInfo*) * entryCount);
    Serial.printf(F("Reading %i cp_info entries\n"), entryCount);
    for(uint16_t entry = 0;entry < entryCount; entry++)
        readEntry(&array[entry], s);

    return array;
}

void ConstantPool::readEntry(CPInfo** info, Stream* s) {
    auto tag = (uint8_t) s->read();
    switch (tag) {
        case CONSTANT_Utf8: // UTF8_info requires a variable amount of memory to hold the bytes
            {
                u2 length = ReadU2(s);
                auto* utf8_info = (CONSTANT_Utf8_info *)(sdmalloc(sizeof(u1)/*tag*/ + sizeof(u2)/*length*/ + sizeof(u1) * length));
                utf8_info->length = length;
                s->readBytes(utf8_info->bytes, length);
                *info = (CPInfo*)utf8_info;
            }
            break;

        case TagTypes::CONSTANT_Class:
            READ_CONSTANT_ENTRY(CONSTANT_Class);
            break;

        case TagTypes::CONSTANT_Fieldref:
            READ_CONSTANT_ENTRY(CONSTANT_Fieldref);
            break;

        case TagTypes::CONSTANT_Methodref:
            READ_CONSTANT_ENTRY(CONSTANT_Methodref);
            break;

        case TagTypes::CONSTANT_InterfaceMethodref:
            READ_CONSTANT_ENTRY(CONSTANT_InterfaceMethodref);
            break;

        case CONSTANT_String:
            READ_CONSTANT_ENTRY(CONSTANT_String);
            break;

        case CONSTANT_Integer:
            READ_CONSTANT_ENTRY(CONSTANT_Integer)
            break;

        case CONSTANT_Float:
            READ_CONSTANT_ENTRY(CONSTANT_Float);
            break;

        case CONSTANT_Long:
            READ_CONSTANT_ENTRY(CONSTANT_Long);
            break;

        case CONSTANT_Double:
            READ_CONSTANT_ENTRY(CONSTANT_Double);
            break;

        case CONSTANT_NameAndType:
            READ_CONSTANT_ENTRY(CONSTANT_NameAndType);
            break;


        case CONSTANT_MethodHandle:
            READ_CONSTANT_ENTRY(CONSTANT_MethodHandle);
            break;

        case CONSTANT_MethodType:
            READ_CONSTANT_ENTRY(CONSTANT_MethodType);
            break;

        case CONSTANT_InvokeDynamic:
            READ_CONSTANT_ENTRY(CONSTANT_InvokeDynamic);
            break;

        default:
            Serial.printf("[Error!] Unknown constant tag %i\n", tag);
            // TODO: error
            break;
    }
    (*info)->tag = tag;
}
