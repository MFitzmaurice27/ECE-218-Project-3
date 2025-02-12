//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#define NUM_READINGS        10

//=====[Declaration and initialization of public global objects]===============

DigitalIn ignition(BUTTON1);
DigitalIn passengerInSeat(D4);
DigitalIn driverInSeat(D5);
DigitalIn passengerSeatbelt(D6);
DigitalIn driverSeatbelt(D7);
AnalogIn potentiometer(A0);

DigitalOut ignitionEnabled(LED1);
DigitalOut engineStarted(LED2);
DigitalInOut buzzer(PE_10);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============

bool driverSeated;
int waitForStop = 0;

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();

void driverCheck();
void seatbeltCheck();
void engineStart();
void engineStop();

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    inputsInit();
    outputsInit();
    driverSeated = false;

    while (true) {

        // Checks if Driver is in the seat
        driverCheck();

        // Both seats occupied and seatbelts fastened
        seatbeltCheck();

        // Checks if engine can start
        engineStart();

        // Stops engine if ignition is pressed and released
        engineStop();
    }
}

//=====[Implementations of public functions]===================================

void inputsInit()
{
    ignitionEnabled = OFF;
    engineStarted = OFF;

    buzzer.mode(OpenDrain);
    buzzer.input();
}

void outputsInit()
{
    ignition.mode(PullDown);
    driverInSeat.mode(PullDown);
    passengerInSeat.mode(PullDown);
    driverSeatbelt.mode(PullDown);
    passengerSeatbelt.mode(PullDown);
}

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

        buzzer.input();

        waitForStop = 1;
        
    } else if (ignitionEnabled == OFF && ignition == ON && !engineStarted) {
        uartUsb.write("Ignition inhibited\n", 19);

        buzzer.output();
        buzzer = LOW;

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
