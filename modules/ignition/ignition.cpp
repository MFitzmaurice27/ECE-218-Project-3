//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "ignition.h"

#include "buzzer.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================


//=====[Declaration and initialization of public global objects]===============

DigitalIn ignition(BUTTON1);
DigitalIn passengerInSeat(D10);
DigitalIn driverInSeat(D11);
DigitalIn passengerSeatbelt(D12);
DigitalIn driverSeatbelt(D13);

DigitalOut ignitionEnabled(LED1);
DigitalOut engineStarted(LED2);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

bool driverSeated = false;
int waitForStop = 0;

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

void ignitionInit() {
    ignition.mode(PullDown);
    driverInSeat.mode(PullDown);
    passengerInSeat.mode(PullDown);
    driverSeatbelt.mode(PullDown);
    passengerSeatbelt.mode(PullDown);

    ignitionEnabled = OFF;
    engineStarted = OFF;
    
    buzzerInit();
}

//=====[Implementations of private functions]==================================

// Checks if the driver is in their seat
void driverCheck() {
    if (driverInSeat && !driverSeated && !engineStarted) {
        uartUsb.write("Welcome to enhanced alarm system model 218-W24\n", 47);
        driverSeated = true;
    } 

    if (!driverInSeat) {
        driverSeated = false;
    }
}

// Checks if both seatbelts are fastened and seats are filled
void seatbeltCheck() {
    if (driverInSeat && passengerInSeat && driverSeatbelt && passengerSeatbelt && !engineStarted) {
        ignitionEnabled = ON;
    } else {
        ignitionEnabled = OFF;
    }
}

// Starts the engine if all conditions are met and ignition is pressed, sounds the alarm otherwise
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

// Stops the engine if its already running and the ignition is pressed and released
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
