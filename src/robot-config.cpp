#include "vex.h"

using namespace vex;

// main
brain  Brain;
controller Controller1;
inertial InertialSensor(PORT8);

// drive train motors
motor leftDriveA(PORT1, ratio18_1, false);
motor leftDriveB(PORT2, ratio18_1, false);
motor rightDriveA(PORT3, ratio18_1, true);
motor rightDriveB(PORT4, ratio18_1, true);

// catapult motor
motor catapultA(PORT5, ratio18_1, false);
motor catapultB(PORT6, ratio18_1, true);

// intake motor
motor intake(PORT7, ratio18_1, false);

void hwInit( void ) {
    printf("Hardware Init\n");

    // calibrate the Inertial Sensor
    InertialSensor.calibrate();
    while (InertialSensor.isCalibrating()) {
        wait(100, msec);
    }
}