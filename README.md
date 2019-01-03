TinyJVM
=======
This project aims at providing a JVM that can be run on microcontrollers (tested with a Teensy 3.6)

In contrary to PC JVMs, this one does not provide any security features (such as bytecode validation before running, etc.) to save time and memory


Usage
-----
Teensy/Arduino example code:
```c++
#include "Arduino.h"
#include <bytecode/ClassFile.h>
#include <vm/TinyJVM.h>

static TinyJVM* jvm = new TinyJVM(); // it is highly recommended to make the JVM object static to save memory

void setup() {
    // [...] initialize 'bytecodeStream'
    ClassFile* classFile = ClassFile::read(bytecodeStream);
    jvm->loadClassFile(*classFile);
    jvm->startFrom(u8"HelloWorldClass", u8"main");
}

void loop() {
    jvm->step();
}
```