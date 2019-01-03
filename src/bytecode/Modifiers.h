//
// Created by jglrxavpok on 01/01/19.
//

#ifndef TINYJVM_MODIFIERS_H
#define TINYJVM_MODIFIERS_H

#include <stdint.h>

constexpr uint16_t ACC_PUBLIC = 0x1;
constexpr uint16_t ACC_PRIVATE = 0x2;
constexpr uint16_t ACC_PROTECTED = 0x4;
constexpr uint16_t ACC_STATIC = 0x8;
constexpr uint16_t ACC_FINAL = 0x10;
constexpr uint16_t ACC_SUPER = 0x20;
constexpr uint16_t ACC_VOLATILE = 0x40;
constexpr uint16_t ACC_TRANSIENT = 0x80;
constexpr uint16_t ACC_INTERFACE = 0x200;
constexpr uint16_t ACC_ABSTRACT = 0x400;
constexpr uint16_t ACC_SYNTHETIC = 0x1000;
constexpr uint16_t ACC_ANNOTATION = 0x2000;
constexpr uint16_t ACC_ENUM = 0x4000;

#endif //TINYJVM_MODIFIERS_H
