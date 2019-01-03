//
// Created by jglrxavpok on 01/01/19.
//

#include "Arduino.h"
#include "ClassFile.h"
#include "ioutils.h"

ClassFile* ClassFile::read(Stream* s) {
    uint32_t magic = ReadU4(s);
    if(magic != 0xCAFEBABE) {
        Serial.printf("Not magic number ;( is %lx read error: %i\n", magic, s->getReadError());
        return nullptr;
    }

    uint16_t minorVersion = ReadU2(s);
    uint16_t majorVersion = ReadU2(s);
    uint16_t constantPoolCount = ReadU2(s);


    Serial.println("Starting to read constant pool");
    CPInfo** constantPool = ConstantPool::read(constantPoolCount-1, s);

    uint16_t accessFlag = ReadU2(s);
    uint16_t thisClass = ReadU2(s);
    uint16_t superClass = ReadU2(s);
    uint16_t interfacesCount = ReadU2(s);
    uint16_t interfaces[interfacesCount];
    Serial.println("Starting to read interfaces");
    ReadU2Array(s, interfaces, interfacesCount);

    uint16_t fieldCount = ReadU2(s);
    Serial.println("Starting to read fields");
    FieldInfo* fields = FieldInfo::read(fieldCount, s);
    uint16_t methodCount = ReadU2(s);
    Serial.println("Starting to read methods");
    MethodInfo* methods = MethodInfo::read(methodCount, s);
    uint16_t attributeCount = ReadU2(s);
    Serial.println("Starting to read attributes");
    AttributeInfo* attributes = AttributeInfo::read(attributeCount, s);

    auto* classFile = new ClassFile(magic, minorVersion, majorVersion, constantPoolCount, constantPool, accessFlag, thisClass, superClass, interfacesCount, interfaces, fieldCount, fields, methodCount, methods, attributeCount, attributes);
    return classFile;
}

ClassFile::ClassFile(uint32_t magicNumber, uint16_t minor, uint16_t major, uint16_t constantPoolCount,
                     CPInfo **constantPool, uint16_t accessFlags, uint16_t thisClass, uint16_t superClass,
                     uint16_t interfaceCount, uint16_t *interfaces, uint16_t fieldCount, FieldInfo *fields,
                     uint16_t methodCount, MethodInfo *methods, uint16_t attributeCount, AttributeInfo *attributes):
                     magicNumber(magicNumber), minorVersion(minor), majorVersion(major), constantPoolCount(constantPoolCount), constantPool(constantPool), accessFlags(accessFlags), thisClass(thisClass), superClass(superClass),
                     interfaceCount(interfaceCount), interfaces(interfaces), fieldCount(fieldCount), fields(fields), methodCount(methodCount), methods(methods), attributeCount(attributeCount), attributes(attributes)

                     {

}
