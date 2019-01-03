//
// Created by jglrxavpok on 01/01/19.
//

#include "Arduino.h"
#include "sdmalloc.h"
#include <bytecode/ClassFile.h>
#include <vector>
#include "TinyJVM.h"
#include "CodeAttribute.h"
#include "bytecode/opcodes.h"

TinyJVM::TinyJVM() {
    currentLocation.programCounter = 0;
    currentLocation.currentMethod = nullptr;
    currentLocation.currentClass = nullptr;
}

void TinyJVM::step() {
    if(!currentLocation.currentMethod) { // end of execution or no start point at all
        return;
    }
    u1* code = currentLocation.currentMethod->codeAttribute->getCode();
    size_t& pc = currentLocation.programCounter;
    u1 instruction = code[pc++];
    CPInfo** constants = currentLocation.currentClass->constantPool;

    switch(instruction) {
        case GETSTATIC: {
            size_t index = code[pc++] << 8 | code[pc++];
            auto* fieldInfo = (CONSTANT_Fieldref_info*)constants[index-1];
            auto* nameAndTypeInfo = (CONSTANT_NameAndType_info*)constants[fieldInfo->name_and_type_index-1];
            auto* owningClassInfo = (CONSTANT_Class_info*)constants[fieldInfo->class_index-1];
            utf8_string* owningClassName = ConstantPool::getUTF8(constants, owningClassInfo->name_index);
            utf8_string* fieldName = ConstantPool::getUTF8(constants, nameAndTypeInfo->name_index);
            JClass* clazz = loadedClasses[*owningClassName];
            if(!owningClassName->compare("java/lang/System") && !fieldName->compare("out")) {
                auto* stdoutValue = freeValueStructs.get();
                stdoutValue->data = nullptr;
                stdoutValue->type = parseBasicDescriptor("Ljava/io/PrintStream");
                executionStack.push(*stdoutValue);
            }
            Serial.printf("GETSTATIC %s %s (class ptr is %p)\n", owningClassName->data(), fieldName->data(), clazz);
        }
            break;

        case LDC: {
            uint8_t index = code[pc++];
            CPInfo* constant = constants[index-1];
            switch(constant->tag) {
                case CONSTANT_String: {
                    auto* strInfo = (CONSTANT_String_info*)constant;
                    auto* strValue = stringValue(ConstantPool::getUTF8(constants, strInfo->string_index));
                    executionStack.push(*strValue);
                }
                    break;

                default:
                    Serial.printf("Unsupported constant type: %i\n", constant->tag);
                    break;
            }
        }
            break;

        case INVOKEVIRTUAL: {
            size_t index = code[pc++] << 8 | code[pc++];
            auto* methodInfo = (CONSTANT_Methodref_info*)constants[index-1];
            auto* nameAndTypeInfo = (CONSTANT_NameAndType_info*)constants[methodInfo->name_and_type_index-1];
            auto* owningClassInfo = (CONSTANT_Class_info*)constants[methodInfo->class_index-1];
            utf8_string* owningClassName = ConstantPool::getUTF8(constants, owningClassInfo->name_index);
            utf8_string* methodName = ConstantPool::getUTF8(constants, nameAndTypeInfo->name_index);
            Serial.printf("INVOKEVIRTUAL %s %s\n", owningClassName->data(), methodName->data());

            if(!methodName->compare(u8"println") && !owningClassName->compare(u8"java/io/PrintStream")) {
                Value stringValue = executionStack.top();
                freeValueStructs.store(&stringValue);
                executionStack.pop();
                Value printStream = executionStack.top();
                executionStack.pop();
                auto* str = (utf8_string*)stringValue.data;
                Serial.printf("[OUTPUT] %s\n", str->data());
                freeValueStructs.store(&printStream);
            }
        }
            break;

        case RETURN: {
            freeLocationStructs.store(&currentLocation);
            locals = localStack.top();
            localStack.pop();
            currentLocation = callstack.top();
            callstack.pop();
            Serial.println("RETURN");
        }
            break;

        default:
            Serial.printf("Unsupported instruction: %x %i\n", instruction, pc);
            break;
    }
}

Value* TinyJVM::floatValue(uint32_t bytes) {
    auto* value = freeValueStructs.get();
    value->type = parseBasicDescriptor(u8"F");
    value->data = (void*)bytes;
    return value;
}

Value* TinyJVM::intValue(uint32_t bytes) {
    auto* value = freeValueStructs.get();
    value->type = parseBasicDescriptor(u8"I");
    value->data = (void*)bytes;
    return value;
}

Value* TinyJVM::stringValue(utf8_string *str) {
    auto* strValue = freeValueStructs.get();
    strValue->type = parseBasicDescriptor(u8"Ljava/lang/String;");
    strValue->data = str;
    return strValue;
}

void TinyJVM::loadClass(JClass* clazz) {
    loadedClasses.insert({clazz->name, clazz});
}

void TinyJVM::loadClassFile(ClassFile &file) {
    CPInfo** constants = file.constantPool;
    auto* classInfo = (CONSTANT_Class_info*) constants[file.thisClass-1];
    auto* superClassInfo = (CONSTANT_Class_info*) constants[file.superClass-1];
    Serial.printf("Getting names...");
    Serial.printf("Location in memory: %p\n", &file);
    Serial.printf("Index of classname: %i (tag = %i)\n", file.thisClass, constants[file.thisClass]->tag);
    Serial.printf("Index of super classname: %i\n", file.superClass);

    const utf8_string* name = ConstantPool::getUTF8(constants, classInfo->name_index);
    const utf8_string* supername = ConstantPool::getUTF8(constants, superClassInfo->name_index);

    Serial.printf("Loading class %s\n", name->data());
    Serial.printf("\t (super class is %s)\n", supername->data());
    Serial.printf("\t (magic number is %lx)\n", file.magicNumber);
    Serial.printf("\t (version is %i.%i)\n", file.majorVersion, file.minorVersion);

    // build field map
    /*{TEST_MEMORY}
    {TEST_MEMORY}*/
    auto* fieldMap = new FieldMap;
    buildFieldMap(file, *fieldMap);
    auto* methodMap = new MethodMap;
    buildMethodMap(file, *methodMap);
    auto* jClass = new JClass(file.accessFlags, *name, *supername, *fieldMap, *methodMap, file.constantPool);
    loadedClasses.insert({ *name, jClass });
}

void TinyJVM::buildMethodMap(ClassFile& file, MethodMap& map) {
    Serial.println(F("Building method map..."));
    for (uint16_t methodIndex = 0; methodIndex < file.methodCount; methodIndex++) {
        MethodInfo methodInfo = file.methods[methodIndex];
        Serial.printf("\tLoading method %i (name index = %i, descriptorIndex index = %i)\n", methodIndex, methodInfo.nameIndex, methodInfo.descriptorIndex);
        utf8_string* methodName = ConstantPool::getUTF8(file.constantPool, methodInfo.nameIndex);
        utf8_string* descriptor = ConstantPool::getUTF8(file.constantPool, methodInfo.descriptorIndex);

        Serial.printf("\tLoading method %p %p\n", methodName, descriptor);
        Serial.printf("\tLoading method %s %s\n", methodName->data(), descriptor->data());

        CodeAttribute* codeAttribute = nullptr;
        // find code attribute
        // TODO: check if native or abstract first
        for(uint16_t attributeIndex = 0;attributeIndex < methodInfo.attributeCount; attributeIndex++) {
            AttributeInfo info = methodInfo.attributes[attributeIndex];
            utf8_string* attribute_name = ConstantPool::getUTF8(file.constantPool, info.nameIndex);
            if(!attribute_name->compare(u8"Code")) {
                Serial.println("[Debug] Found code attribute");
                codeAttribute = new CodeAttribute(info);
                Serial.println("[Debug] Finished loading code attribute");
            } else {
                Serial.printf("[Debug] Attribute name is %s, not 'Code'\n", attribute_name->data());
            }
        }

        if(!codeAttribute) {
            Serial.printf("No code attribute found for method %s\n", methodName->data());
        }

        JMethod* method = new JMethod(methodInfo.accessFlags, *methodName, *parseMethodDescriptor(*descriptor), codeAttribute);
        map.insert({ *methodName, method });
        Serial.printf("Finished loading method %s\n", methodName->data());
    }
}

void TinyJVM::buildFieldMap(ClassFile& file, FieldMap& map) {
    Serial.println("Building field map...");
    for (uint16_t fieldIndex = 0; fieldIndex < file.fieldCount; fieldIndex++) {
        FieldInfo fieldInfo = file.fields[fieldIndex];
        utf8_string* fieldName = ConstantPool::getUTF8(file.constantPool, fieldInfo.nameIndex);
        utf8_string* descriptor = ConstantPool::getUTF8(file.constantPool, fieldInfo.descriptorIndex);
        JField* field = new JField(fieldInfo.accessFlags, *fieldName, *parseBasicDescriptor(*descriptor));
        utf8_string str = *fieldName;
        map.insert({ str, field });
    }
}

const BasicDescriptor* TinyJVM::parseBasicDescriptor(const utf8_string& str, size_t* cursor) {
    //Serial.printf("parseBasicDescriptor %s %p\n", str.data(), cursor);
    if(basicDescriptors[str] != nullptr) {
        const BasicDescriptor* desc = basicDescriptors[str];
        if(cursor)
            *cursor = *cursor + desc->descLength;
        return desc;
    }
    const BasicDescriptor* descriptor = actualParseBasicDescriptor(str, cursor);
    basicDescriptors[str] = descriptor;
    return basicDescriptors[str];
}

const MethodDescriptor* TinyJVM::parseMethodDescriptor(const utf8_string& str) {
    if(methodDescriptors[str] != nullptr) {
        //Serial.printf("Cache hit for %s\n", str.data());
        return methodDescriptors[str];
    }
    //Serial.printf("Cache miss for %s\n", str.data());
    if(str[0] != '(') {
        // TODO: log
        Serial.printf("Invalid method descriptor: %s\n", str.data());
        return nullptr;
    }
    size_t index = 1; // skip '('
    std::vector<const BasicDescriptor*> args;
    const BasicDescriptor* returnType = nullptr;
    do {
       // Serial.printf("index = %i (remaining = %s)\n", index, str.substr(index).data());
        if(str[index] == 'L') {
            size_t semicolonIndex = str.find(";", index);
            utf8_string name = str.substr(index, semicolonIndex-index+1);
            const BasicDescriptor* arg = parseBasicDescriptor(name, &index); // name = 'L$type name$;' at this point
            args.push_back(arg);
        } else if(str[index] == ')') {
            index++;
            returnType = parseBasicDescriptor(str, &index);
            break;
        } else {
            const BasicDescriptor* arg = parseBasicDescriptor(str, &index);
            args.push_back(arg);
        }
    } while(index < str.length());
    auto* descriptor = (MethodDescriptor*)sdmalloc(sizeof(BasicDescriptor*) + sizeof(uint32_t) + args.size()*sizeof(BasicDescriptor*));
    descriptor->argCount = args.size();
    descriptor->returnType = returnType;
    descriptor->argumentTypes = args.data();
    methodDescriptors[str] = descriptor;
    return descriptor;
}

const BasicDescriptor *TinyJVM::actualParseBasicDescriptor(const utf8_string& str, size_t* cursor) {
    size_t start = 0;
    if(cursor)
        start = *cursor;
    else
        cursor = &start;
   // Serial.printf(">> At start(%i) %c\n", start, str[start]);
    auto* descriptor = (BasicDescriptor*)sdmalloc(sizeof(BasicDescriptor));
    descriptor->isNative = str[start] != 'L' && str[start] != '[';
    descriptor->isArray = str[start] == '[';
    if(descriptor->isNative) {
        switch (str[start]) {
            case 'Z':
                descriptor->nativeType = BOOLEAN;
                break;

            case 'V':
                descriptor->nativeType = VOID;
                break;

            case 'J':
                descriptor->nativeType = LONG;
                break;

            case 'I':
                descriptor->nativeType = INTEGER;
                break;

            case 'F':
                descriptor->nativeType = FLOAT;
                break;

            case 'B':
                descriptor->nativeType = BYTE;
                break;

            case 'C':
                descriptor->nativeType = CHAR;
                break;

            case 'D':
                descriptor->nativeType = DOUBLE;
                break;

            case 'S':
                descriptor->nativeType = SHORT;
                break;

            default:
                Serial.printf("[Error] %c is not a valid native type\n", str[start]);
                descriptor->nativeType = INVALID;
                break;
        }
        if(cursor)
            *cursor = *cursor +1;
    } else if(descriptor->isArray) {
        if(cursor) {
            *cursor = *cursor+1;
        }
       // Serial.printf("preread array %i %s\n", str.size(), str.c_str());
       // Serial.printf("read array %p\n", &str);
        descriptor->elementType = parseBasicDescriptor(str, cursor);
    } else {
        size_t semicolonIndex = str.find(";", 1);
        utf8_string name = str.substr(1, semicolonIndex-1);
        descriptor->className = &name;
        if(cursor) {
            *cursor = *cursor + semicolonIndex-1 +1/*semicolon*/;
        }
    }
    descriptor->descLength = *cursor- start;
  //  Serial.printf("Read basic descriptor %s\n", str.data());
    return descriptor;
}

void TinyJVM::startFrom(utf8_string className, utf8_string mainMethodName) {
    JClass* jClass = loadedClasses[className];
    JMethod* method = (jClass->methodMap.find(mainMethodName))->second;
    if(!method) {
        Serial.printf("Could not find method %s ([Ljava/lang/String;)V in class %s!\n", mainMethodName.data(), className.data());
        Serial.println("Here are the available methods:");
        for (auto const& x : jClass->methodMap)
        {
            Serial.printf("\t'%s'\n", x.first.data());
        }
        return;
    }
    currentLocation.currentMethod = method;
    currentLocation.currentClass = jClass;
    currentLocation.programCounter = 0;
    Serial.printf("Found main method!!!! Max stack = %i; Max Locals = %i; code length = %li\n", method->codeAttribute->getMaxStack(), method->codeAttribute->getMaxLocals(), method->codeAttribute->getCodeLength());
}
