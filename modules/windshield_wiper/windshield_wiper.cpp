//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "windshield_wiper.h"
#include "display.h"
#include "ignition.h"

//=====[Constants]============================================================

#define DUTY_MIN 0.05
#define DUTY_MAX 0.080
#define PERIOD 0.02 // Period in seconds

#define INT_SHORT_DELAY 2000
#define INT_MEDIUM_DELAY 4000
#define INT_LONG_DELAY 6000

#define DISPLAY_UPDATE_INTERVAL 200 // Reduced display refresh time

//=====[Global Objects]=======================================================

PwmOut wiperServo(D15);
AnalogIn modeSelector(A0);
AnalogIn intSelector(A1);

//=====[Global Variables]=====================================================

extern DigitalOut engineStarted;
static bool wiperState = false;
int accumulatedWiperTime = 0;
int accumulatedDisplayTime = 0;

//=====[Function Prototypes]==================================================

static void displayWiperMode(const char* mode, const char* delay);
static void servoSpeed(int speedDelay); // Renamed function prototype

//=====[Function Implementations]=============================================

void windshieldWiperInit() {
    wiperServo.period(PERIOD);
    wiperServo.write(DUTY_MIN);
    displayWiperMode("OFF", "");
}

void windshieldWiperUpdate() {
    if (!engineStarted) {
        displayWiperMode("OFF", "");
        wiperServo.write(DUTY_MIN);
        return;
    }

    float modeValue = modeSelector.read();
    float intValue = intSelector.read();
    
    const char* mode;
    const char* delay = "";
    int delayTime = 0;
    int cycleTime = 0;

    if (modeValue < 0.25) {
        mode = "OFF";
        wiperServo.write(DUTY_MIN);
    } else if (modeValue < 0.5) {
        mode = "LO";
        cycleTime = 1750; // Slow speed for LO mode
    } else if (modeValue < 0.75) {
        mode = "HI";
        cycleTime = 500; // Fast speed for HI mode
    } else {
        mode = "INT";
        if (intValue < 0.33) {
            delayTime = INT_SHORT_DELAY;
            delay = "SHORT";
        } else if (intValue < 0.66) {
            delayTime = INT_MEDIUM_DELAY;
            delay = "MEDIUM";
        } else {
            delayTime = INT_LONG_DELAY;
            delay = "LONG";
        }
        cycleTime = 1500; // Default speed for INT mode
    }

    if (accumulatedWiperTime >= cycleTime && mode != "INT") {
        accumulatedWiperTime = 0;
        servoSpeed(cycleTime); // Use servoSpeed to control speed based on mode
    }

    if (mode == "INT" && !wiperState && accumulatedWiperTime >= delayTime) {
        accumulatedWiperTime = 0;
        servoSpeed(cycleTime); // Use servoSpeed for INT mode
    }

    if (accumulatedDisplayTime >= DISPLAY_UPDATE_INTERVAL) {
        displayWiperMode(mode, delay);
        accumulatedDisplayTime = 0;
    }

    accumulatedDisplayTime = accumulatedDisplayTime + 10;
    accumulatedWiperTime = accumulatedWiperTime + 10;
}

//=====[Private Function Implementations]=====================================

static void displayWiperMode(const char* mode, const char* delay) {
    displayClear();
    displayCharPositionWrite(0, 0);
    displayStringWrite("Wiper Mode: ");
    displayStringWrite(mode);
    if (strcmp(mode, "INT") == 0) {
        displayCharPositionWrite(0, 1);
        displayStringWrite("Delay: ");
        displayStringWrite(delay);
    }
    printf("Wiper Mode: %s, Delay: %s\n", mode, delay);
}

// added function for Speed

static void servoSpeed(int speedDelay) {
    wiperServo.write(DUTY_MAX); // Move to maximum position
    delay(speedDelay / 2);       // Delay for half the cycle time
    wiperServo.write(DUTY_MIN); // Move to minimum position
    delay(speedDelay / 2);       // Delay for half the cycle time
}
