//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

//=====[Defines]===============================================================



//=====[Declaration and initialization of public global objects]===============

DigitalIn ignition(D3);
DigitalIn passengerInSeat(D4);
DigitalIn driverInSeat(D5);
DigitalIn passengerSeatbelt(D6);
DigitalIn driverSeatbelt(D7);

bool driverSeated;

DigitalOut ignitionEnabled(LED1);
DigitalOut engineStarted(LED2);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============



//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();

void uartTask();

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    inputsInit();
    outputsInit();
    driverSeated = false;

    while (true) {

        // Checks if Driver is in the seat
        if (driverInSeat && !driverSeated) {
            uartUsb.write("Welcome to enhanced alarm system model 218-W24\n", 47);
            driverSeated = true;
        } 

        if (!driverInSeat) {
            driverSeated = false;
        }

    }
}

//=====[Implementations of public functions]===================================

void inputsInit()
{
    ignitionEnabled = OFF;
    engineStarted = OFF;
}

void outputsInit()
{
    ignition.mode(PullDown);
    passengerInSeat.mode(PullDown);
    driverInSeat.mode(PullDown);
    passengerSeatbelt.mode(PullDown);
    driverSeatbelt.mode(PullDown);
}
