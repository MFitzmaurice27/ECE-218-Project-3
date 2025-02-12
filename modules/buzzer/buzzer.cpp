//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "buzzer.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

DigitalInOut buzzer(PE_10);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

void buzzerInit() {
    buzzer.mode(OpenDrain);
    buzzer.input();
}

void buzzerOn() {
    buzzer.output();
    buzzer = LOW;
}

void buzzerOff() {
    buzzer.input();
}

//=====[Implementations of private functions]==================================

