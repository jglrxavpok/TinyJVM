//
// Created by jglrxavpok on 02/01/19.
//

#include <ByteStream.h>
#include <ioutils.h>
#include <bytecode/class_format_types.h>
#include "CodeAttribute.h"
#include "sdmalloc.h"

CodeAttribute::CodeAttribute(AttributeInfo& info) {
    Serial.printf("pre stream %p, %li\n", (void*)info.info, info.length);
    auto* stream = new ByteStream(info.info, info.length);

    Serial.println("starting reading code");
    maxStack = ReadU2(stream);
    maxLocals = ReadU2(stream);
    codeLength = ReadU4(stream);
    Serial.println("pre malloc");
    code = (u1*)sdmalloc(sizeof(u1)*codeLength);
    stream->readBytes(code, codeLength);
    Serial.println("end reading code");
    // TODO: extract rest of data

    free(stream);
}

u1* CodeAttribute::getCode() {
    return code;
}

u4 CodeAttribute::getCodeLength() {
    return codeLength;
}

u2 CodeAttribute::getMaxStack() {
    return maxStack;
}

u2 CodeAttribute::getMaxLocals() {
    return maxLocals;
}

