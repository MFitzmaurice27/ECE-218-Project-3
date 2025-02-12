//=====[Libraries]=============================================================

#include "car_system.h"
#include "mbed.h"

#include "ignition.h"
#include "windshield_wiper.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

void carSystemInit() {
    ignitionInit();
}

void carSystemUpdate() {
    ignitionUpdate();
}
