//
// Created by jglrxavpok on 01/01/19.
//

#ifndef TINYJVM_IOUTILS_H
#define TINYJVM_IOUTILS_H

#include <stdint.h>
#include <Stream.h>

uint16_t ReadU2(Stream*);
uint32_t ReadU4(Stream*);
void ReadU2Array(Stream* stream, uint16_t* array, size_t count);
void ReadU4Array(Stream* stream, uint32_t* array, size_t count);
#endif //TINYJVM_IOUTILS_H
