//
// Created by jglrxavpok on 02/01/19.
//

#include <ioutils.h>
#include <sdmalloc.h>
#include "AttributeInfo.h"

void AttributeInfo::readEntry(AttributeInfo* entry, Stream* s) {
    entry->nameIndex = ReadU2(s);
    entry->length = ReadU4(s);
    entry->info = (uint8_t*) sdmalloc(entry->length*sizeof(uint8_t));
    s->readBytes(entry->info, entry->length);
}

AttributeInfo* AttributeInfo::read(uint16_t count, Stream* s) {
    auto* array = (AttributeInfo*) sdmalloc(count*sizeof(AttributeInfo));
    for(uint16_t i = 0; i < count; i++) {
        readEntry(&array[i], s);
    }
    return array;
}