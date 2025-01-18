//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

//=====[Declaration and initialization of public global objects]===============

DigitalIn ignition(D3);
DigitalIn passengerInSeat(D4);
DigitalIn driverInSeat(D5);
DigitalIn passengerSeatbelt(D6);
DigitalIn driverSeatbelt(D7);

DigitalOut ignitionEnabled(LED1);
DigitalOut engineStarted(LED2);
DigitalInOut buzzer(PE_10);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============

bool driverSeated;
bool fail = false;

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();

void driverCheck();
void seatbeltCheck();
void engineStart();

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
}

void driverCheck() {
    if (driverInSeat && !driverSeated && !fail && !engineStarted) {
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
    } else if (ignitionEnabled == OFF && ignition == ON && !engineStarted && !fail) {
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

        fail = true;
    }
}
