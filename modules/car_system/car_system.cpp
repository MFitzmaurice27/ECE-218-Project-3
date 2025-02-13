//=====[Libraries]=============================================================

#include "arm_book_lib.h"

#include "car_system.h"
#include "mbed.h"
#include "ignition.h"
#include "windshield_wiper.h"
#include "display.h"

//=====[Implementations of public functions]===================================

void carSystemInit() {
    displayInit();
    ignitionInit();
    windshieldWiperInit();
}

void carSystemUpdate() {
    ignitionUpdate();
    windshieldWiperUpdate();
    delay(10);
}
