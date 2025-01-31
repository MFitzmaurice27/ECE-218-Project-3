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
DigitalIn potentiometer(D3);
DigitalIn lightResistor(D2);

DigitalOut ignitionEnabled(LED1);
DigitalOut engineStarted(LED2);
DigitalOut leftLowBeamLamp(D8);
DigitalOut rightLowBeamLamp(D9);
DigitalInOut buzzer(PE_10);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============

bool driverSeated;
int waitForStop = 0;

float lightReadings[NUM_READINGS];
int i = 0;

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();

void driverCheck();
void seatbeltCheck();
void engineStart();
void engineStop();
void headlightUpdate();
float lightSensorAvg();

void lightSensorSample();

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

        // Updates the state of the headlights
        headlightUpdate();

        // Takes a reading from the light resistor
        lightSensorSample();
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

void headlightUpdate() {
    if (engineStarted == ON) {
        if (potentiometer.read() < 0.33) {
            leftLowBeamLamp = 0;
            rightLowBeamLamp = 0;
        } else if (potentiometer.read() >= 0.33 && potentiometer.read() < 0.66) {
            if (lightSensorAvg() < .56) {
                leftLowBeamLamp = 0;
                rightLowBeamLamp = 0;
            } else {
                leftLowBeamLamp = 1;
                rightLowBeamLamp = 1;
            }
        } else {
            leftLowBeamLamp = 1;
            rightLowBeamLamp = 1;
        }
    } else {
        leftLowBeamLamp = 0;
        rightLowBeamLamp = 0;
    }
}

void lightSensorSample() {
    float lightReadingsTotal;
    float lightReadingsAvg;

    if (i == NUM_READINGS - 1) {
        i = 0;
    } else {
        i++;
    }

    lightReadings[i] = lightResistor.read();
}

float lightSensorAvg() {
    float total = 0;

    for (int j = 0; j < NUM_READINGS; j++) {
        total += lightReadings[j];
    } 

    return total / NUM_READINGS;
}
