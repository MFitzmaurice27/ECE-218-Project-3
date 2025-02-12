//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "ignition.h"

#include "buzzer.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

DigitalIn ignition(BUTTON1);
DigitalIn passengerInSeat(D4);
DigitalIn driverInSeat(D5);
DigitalIn passengerSeatbelt(D6);
DigitalIn driverSeatbelt(D7);

DigitalOut ignitionEnabled(LED1);
DigitalOut engineStarted(LED2);

//=====[Declaration and initialization of public global objects]===============

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

void driverCheck();
void seatbeltCheck();
void engineStart();
void engineStop();

//=====[Implementations of public functions]===================================

void ignitionUpdate() {
    driverCheck();
    seatbeltCheck();
    engineStart();
    engineStop();
}

//=====[Implementations of private functions]==================================

void driverCheck() {
    if (driverInSeat && !driverSeated && !engineStarted) {
        uartUsb.write("Welcome to enhanced alarm system model 218-W24\n", 47);
        driverSeated = true;
    } 

    if (!driverInSeat) {
        driverSeated = false;
    }
}

void seatbeltCheck() {
    if (driverInSeat && passengerInSeat && driverSeatbelt && passengerSeatbelt && !engineStarted) {
        ignitionEnabled = ON;
    } else {
        ignitionEnabled = OFF;
    }
}

void engineStart() {
    if (ignitionEnabled == ON && ignition == ON) {
        engineStarted = ON;
        ignitionEnabled = OFF;
        uartUsb.write("Engine Started\n", 15);

        buzzerOff();

        waitForStop = 1;
        
    } else if (ignitionEnabled == OFF && ignition == ON && !engineStarted) {
        uartUsb.write("Ignition inhibited\n", 19);

        buzzerOn();

        if (!passengerInSeat) {
            uartUsb.write("Passenger seat not occupied\n", 28);
        }

        if (!driverInSeat) {
            uartUsb.write("Driver seat not occupied\n", 25);
        }

        if (!passengerSeatbelt) {
            uartUsb.write("Passenger seatbelt not fastened\n", 32);
        }

        if (!driverSeatbelt) {
            uartUsb.write("Driver seatbelt not fastened\n", 29);
        }
    }
}

void engineStop() {
    switch (waitForStop) {
        case 1: 
            if (!ignition) {
                waitForStop = 2;
            }
            break;
        case 2:
            if (engineStarted && ignition) {
                waitForStop = 3;
            } 
            break;
        case 3:
            if (engineStarted && waitForStop && !ignition) {
                engineStarted = false;
                waitForStop = 0;
            }
            break;
        default: 
            break;
    }
}
