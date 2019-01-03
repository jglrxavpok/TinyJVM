//
// Created by jglrxavpok on 01/01/19.
//

#include "Arduino.h"
#include <bytecode/ClassFile.h>
#include <vm/TinyJVM.h>
#include "SD.h"
#include "Gameboy.class.h"
#include "ByteStream.h"
#include "Main.class.h"


static TinyJVM* jvm = new TinyJVM();

static Sd2Card card;
static SdVolume volume;
static SdFile root;

void setup() {
    Serial.begin(9600);

    while(!Serial);

    Serial.print("\nInitializing SD card...");

    // we'll use the initialization code from the utility libraries
    // since we're just testing if the card is working!
    if (!card.init(SPI_HALF_SPEED, BUILTIN_SDCARD)) {
        Serial.println("initialization failed. Things to check:");
        Serial.println("* is a card inserted?");
        Serial.println("* is your wiring correct?");
        Serial.println("* did you change the chipSelect pin to match your shield or module?");
        return;
    } else {
        Serial.println("Wiring is correct and a card is present.");
    }

    SD.begin(BUILTIN_SDCARD);
    Serial.println("Starting...");

    const char* filepath = "HELLOWOR.CLS";
    File file = SD.open(filepath, FILE_WRITE);
    uint32_t error = file.write(Main_class, Main_class_len);
    file.flush();
    file.close();

    Serial.printf("Copie finie! (error: %i, exists: %i, written bytes: %i)\n", file.getWriteError(), SD.exists(filepath), error);


    file = SD.open(filepath, FILE_READ);
    file.close();

    ByteStream* stream = new ByteStream(Main_class, Main_class_len);

    Serial.println("Class file read!");

    ClassFile* classFile = ClassFile::read(stream);
    jvm->loadClassFile(*classFile);
    Serial.println("Java class loaded!");
    jvm->startFrom(u8"Main", u8"main");
}

void loop() {
    jvm->step();
}
