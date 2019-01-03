//
// Created by jglrxavpok on 02/01/19.
//

#include "Arduino.h"
#include <ioutils.h>
#include <sdmalloc.h>
#include "FieldInfo.h"

void FieldInfo::readEntry(FieldInfo* entry, Stream* s) {
    entry->accessFlags = ReadU2(s);
    entry->nameIndex = ReadU2(s);
    entry->descriptorIndex = ReadU2(s);
    entry->attributeCount = ReadU2(s);
    entry->attributes = AttributeInfo::read(entry->attributeCount, s);
}

FieldInfo* FieldInfo::read(uint16_t count, Stream* s) {
    auto* array = (FieldInfo*) sdmalloc(count*sizeof(FieldInfo));
    for(uint16_t i = 0; i < count; i++) {
        readEntry(&array[i], s);
    }
    return array;
}