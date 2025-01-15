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
    while (true) {
        
        uartTask();
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

void uartTask()
{
    char receivedChar = '\0';
    if( uartUsb.readable() ) {
        uartUsb.read( &receivedChar, 1 );
        switch (receivedChar) {
        case '1':

            break;

        case '2':

            break;

        case '3':

            break;
            
        case '4':
                
            break;

        case '5':

            break;

        default:

            break;

        }
    }
}
