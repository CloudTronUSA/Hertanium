#include "vex.h"

using namespace vex;

// main
brain  Brain;
controller Controller1;
inertial InertialSensor(PORT8);

// drive train motors
motor leftDriveA(PORT12, ratio18_1, true);
motor leftDriveB(PORT13, ratio18_1, true);
motor rightDriveA(PORT1, ratio18_1, false);
motor rightDriveB(PORT2, ratio18_1, false);

motor_group LeftMotors(leftDriveA, leftDriveB); 
motor_group RightMotors(rightDriveA, rightDriveB);

// catapult motor
motor catapultA(PORT5, ratio18_1, false);
motor catapultB(PORT6, ratio18_1, true);

// intake motor
motor intake(PORT7, ratio18_1, false);

void hwInit( void ) {
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("= System Booting...");
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1,1);
    Controller1.Screen.print("= SYSTEM BOOTING =");
    Controller1.Screen.setCursor(2,1);
    Controller1.Screen.print("DONT TOUCH ROBOT");

    // calibrate the Inertial Sensor
    Brain.Screen.setCursor(4, 1);
    Brain.Screen.print("Calibrating Inertial Sensor...");

    InertialSensor.calibrate();
    while (InertialSensor.isCalibrating()) {
        wait(100, msec);
    }

    Brain.Screen.setCursor(4, 1);
    Brain.Screen.print("Calibrating Inertial Sensor... Complete!");

    // setup drivetrain
    smartdrive drivetrain(LeftMotors, RightMotors, InertialSensor, 319.19, 415, 100, mm, 1.0);
    
    Brain.Screen.setCursor(5, 1);
    Brain.Screen.print("Drivetrain ready!");
}