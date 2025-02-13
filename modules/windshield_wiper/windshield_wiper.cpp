//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "windshield_wiper.h"
#include "display.h"
#include "ignition.h"

//=====[Constants]============================================================

#define DUTY_MIN 0.025
#define DUTY_MAX 0.125
#define PERIOD 0.02 // Period in seconds

#define INT_SHORT_DELAY 3000
#define INT_MEDIUM_DELAY 6000
#define INT_LONG_DELAY 8000

#define DISPLAY_UPDATE_INTERVAL 200 // Reduced display refresh time

//=====[Global Objects]=======================================================

PwmOut wiperServo(D15);
AnalogIn modeSelector(A0);
AnalogIn intSelector(A1);

Timer displayTimer;
Timer wiperTimer;

//=====[Global Variables]=====================================================

extern DigitalOut engineStarted;
static bool wiperState = false;

//=====[Function Prototypes]==================================================

static void returnToZero();
static void displayWiperMode(const char* mode, const char* delay);
static float readModeSelector();
static float readIntSelector();

//=====[Function Implementations]=============================================

void windshieldWiperInit() {
    wiperServo.period(PERIOD);
    wiperServo.write(DUTY_MIN);
    displayTimer.start();
    wiperTimer.start();
}

void windshieldWiperUpdate() {
    if (!engineStarted) {
        wiperServo.write(DUTY_MIN);
        if (displayTimer.read_ms() >= DISPLAY_UPDATE_INTERVAL) {
            displayWiperMode("OFF", "");
            displayTimer.reset();
        }
        return;
    }

    float modeValue = readModeSelector();
    float intValue = readIntSelector();
    
    const char* mode;
    const char* delay = "";
    int delayTime = 0;
    int cycleTime = 0;

    if (modeValue < 0.25) {
        mode = "OFF";
        returnToZero();
    } else if (modeValue < 0.5) {
        mode = "LO";
        cycleTime = 1500;
    } else if (modeValue < 0.75) {
        mode = "HI";
        cycleTime = 500;
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
        cycleTime = 1500;
    }

    if (wiperTimer.read_ms() >= cycleTime) {
        wiperTimer.reset();
        wiperState = !wiperState;
        if (wiperState) {
            wiperServo.write(DUTY_MAX);
        } else {
            wiperServo.write(DUTY_MIN);
        }
    }

    if (mode == "INT" && !wiperState && wiperTimer.read_ms() >= delayTime) {
        wiperTimer.reset();
    }

    if (displayTimer.read_ms() >= DISPLAY_UPDATE_INTERVAL) {
        displayWiperMode(mode, delay);
        displayTimer.reset();
    }
}

//=====[Private Function Implementations]=====================================

static void returnToZero() {
    wiperServo.write(DUTY_MIN);
}

static float readModeSelector() {
    return modeSelector.read();
}

static float readIntSelector() {
    return intSelector.read();
}

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