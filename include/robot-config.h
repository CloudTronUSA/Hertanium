using namespace vex;

extern brain Brain;
extern controller Controller1;
extern inertial InertialSensor;

extern motor leftDriveA;
extern motor leftDriveB;
extern motor rightDriveA;
extern motor rightDriveB;

extern motor_group LeftMotors;
extern motor_group RightMotors;

extern motor catapultA;
extern motor catapultB;

extern motor intake;

extern smartdrive drivetrain;

void  hwInit( void );