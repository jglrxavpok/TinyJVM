//
// Created by jglrxavpok on 02/01/19.
//

#include <ioutils.h>
#include "MethodInfo.h"
#include "sdmalloc.h"

void MethodInfo::readEntry(MethodInfo* entry, Stream* s) {
    entry->accessFlags = ReadU2(s);
    entry->nameIndex = ReadU2(s);
    entry->descriptorIndex = ReadU2(s);
    entry->attributeCount = ReadU2(s);
    entry->attributes = AttributeInfo::read(entry->attributeCount, s);
}

MethodInfo* MethodInfo::read(uint16_t count, Stream* s) {
    auto* array = (MethodInfo*) sdmalloc(count*sizeof(MethodInfo));
    for(uint16_t i = 0; i < count; i++) {
        readEntry(&array[i], s);
    }
    return array;
}