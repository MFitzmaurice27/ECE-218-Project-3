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

void driverCheck();
void seatbeltCheck();

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
    driverInSeat.mode(PullDown);
    driverSeatbelt.mode(PullDown);
}

void driverCheck() {
    if (driverInSeat && !driverSeated) {
        uartUsb.write("Welcome to enhanced alarm system model 218-W24\n", 47);
        driverSeated = true;
    } 

    if (!driverInSeat) {
        driverSeated = false;
    }
}

void seatbeltCheck() {
    if (driverInSeat && passengerInSeat && driverSeatbelt && passengerSeatbelt) {
        ignitionEnabled = ON;
    } else {
        ignitionEnabled = OFF;
    }
}
