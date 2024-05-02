#include "vex.h"
#include "gifclass.h"
#include "display.h"
#include <algorithm>

using namespace vex;

// cycle time monitoring
float cycleTime = 0;
float cyclePerSecond = 0;
double cycleBegin = 0;
bool inverseCtrl = false;
bool cltrOverrride = false;
bool preciseCltr = false;

double buttonXLastPressed = 0;
double buttonL1LastPressed = 0;
double buttonR1LastPressed = 0;

DirectionDisplay dirDisplay(150, 120, 80);
competition Competition;

// Display update
void displayUpdate(double actualAngle, int actualSpeed, double expectedAngle, int expectedSpeed) {
    // cps
    Brain.Screen.printAt(1, 40, "T/%0.2f ms", cycleTime);
    Brain.Screen.printAt(1, 20, "C/%0.2f", cyclePerSecond);

    // direction display
    dirDisplay.update(actualAngle, actualSpeed, expectedAngle, expectedSpeed);
}

// Update motor
void motorUpdate(int speed, double angle) {
    double radians = (angle + 90) * (M_PI / 180.0);  // Convert angle to radians
    double cosineValue = cos(radians);

    // Convert cosine output (-1 to 1) to a percentage (0% to 200%)
    // +100 to shift range from -1 to 1, to 0 to 2
    // *100 to convert 0 to 2 range into 0% to 200%
    double speedDiff = (1 - cosineValue) * 100;  // Adjust calculation here

    double diffFactor = (speedDiff - 100) / 100.0;

    printf("D: %f", diffFactor);

    double leftSpeed = speed * (1 + diffFactor);
    double rightSpeed = speed * (1 - diffFactor);

    if (leftSpeed > 100) {
        rightSpeed -= (leftSpeed - 100);
        leftSpeed = 100;
    } else if (rightSpeed > 100) {
        leftSpeed -= (rightSpeed - 100);
        rightSpeed = 100;
    }
    
    LeftMotors.setVelocity(leftSpeed, percent);
    RightMotors.setVelocity(rightSpeed, percent);

    LeftMotors.spin(forward);
    RightMotors.spin(forward);

    printf("Left: %f, Right: %f\n", leftSpeed, rightSpeed);
}

void intakeUpdate(int status) {
    intake.setVelocity(100, pct);
    if (status == 1)
        intake.spin(vex::forward);
    else if (status == -1)
        intake.spin(vex::reverse);
    else
        intake.stop();
}

void userctl(void) {
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1,1);
    Controller1.Screen.print("^ RELEASE   INTAKE ^");
    Controller1.Screen.setCursor(2,1);
    Controller1.Screen.print("PRECISE -   - OVRIDE");
    Controller1.Screen.setCursor(4,1);
    Controller1.Screen.print("IN-VERT -   ? PLCHLD");

    int itkStatus = 0;

    Brain.Screen.clearScreen();
    Gif HOSDriveStart("HOSDriveStart.gif", 0, 0 );

    while(1) {
        cycleBegin = vex::timer::system();

        // actual code
        // Gyro rate in degrees per second (angular velocity)
        float actAngle = InertialSensor.heading();
        float actSpeed = InertialSensor.acceleration(vex::xaxis);
        
        Brain.Screen.setCursor(20, 20);
        Brain.Screen.print("Angle Rate: %.2f deg/s", actAngle);
        Brain.Screen.setCursor(20, 1);
        Brain.Screen.print("X-Axis Speed: %.2f m/s^2", actSpeed);

        // Get angle and magnitude from the controller's joysticks
        double expAngle = atan2(Controller1.Axis1.position(), Controller1.Axis2.position()) * (180 / 3.14159265);
        int expSpeed = 0;
        if (cltrOverrride) {
            expSpeed = sqrt(pow(Controller1.Axis1.position(), 2) + pow(Controller1.Axis2.position(), 2));
            if (Controller1.Axis1.position() < 0)
                expSpeed *= -1;
        }
        else {
            expSpeed = Controller1.Axis3.position();
        }
        //expSpeed = Controller1.Axis3.position(); 
        // Limit speed to 100 (max value for display purposes)
        expSpeed *= 0.5;
        expSpeed = std::min(expSpeed, 100);

        //expSpeed = expSpeed * (Controller1.Axis2.position()/100);

        // Get angle and mag from sensor
        actAngle = InertialSensor.heading();
        actSpeed = InertialSensor.acceleration(vex::xaxis)*10;

        // Get buttons
        // intake
        if (Controller1.ButtonL1.pressing()) {
            if (vex::timer::system() > (buttonL1LastPressed+500)){
                if (itkStatus == 1)
                    itkStatus = 0;
                else if (itkStatus == 0 || itkStatus == -1)
                    itkStatus = 1;
                buttonL1LastPressed = vex::timer::system();
            }
        }
        if (Controller1.ButtonR1.pressing()) {
            if (vex::timer::system() > (buttonR1LastPressed+500)){
                if (itkStatus == -1)
                    itkStatus = 0;
                else if (itkStatus == 0 || itkStatus == 1)
                    itkStatus = -1;
                buttonR1LastPressed = vex::timer::system();
            }
        }

        // right joystisck override
        if (Controller1.ButtonX.pressing()) {
            Controller1.Screen.setCursor(1,1);
            Controller1.Screen.print("^ RELEASE   INTAKE ^");
            Controller1.Screen.setCursor(2,1);
            Controller1.Screen.print("PRECISE -   - OVRIDE");
            Controller1.Screen.setCursor(4,1);
            Controller1.Screen.print("IN-VERT -   ? PLCHLD");

            cltrOverrride = !cltrOverrride;
            Controller1.Screen.setCursor(2,13);
            if (cltrOverrride)
                Controller1.Screen.print("+");
            else
                Controller1.Screen.print("-");
            wait(100, msec);
        }

        // accelerate
        if (Controller1.ButtonL2.pressing()) {
            expSpeed *= 1/0.5;
        }

        printf("Angle: %f, Speed: %d\n", expAngle, expSpeed);

        if (HOSDriveStart.getFrameIndex() > 38) {
            //HOSDriveStart.~Gif();
            displayUpdate(0, 100, expAngle, expSpeed);
        }

        // update motor
        motorUpdate(expSpeed, expAngle);
        intakeUpdate(itkStatus);

        wait(10, msec);

        cycleTime = vex::timer::system() - cycleBegin;
        cyclePerSecond = 1000 / cycleTime;
    }
}

void autonomous(void) {
    LeftMotors.setVelocity(100, pct);
    RightMotors.setVelocity(100, pct);
    LeftMotors.spin(reverse);
    RightMotors.spin(reverse);

    wait(1000, msec);

    LeftMotors.spin(forward);
    RightMotors.spin(forward);

    wait(1000, msec);

    LeftMotors.stop();
    RightMotors.stop();
}

void pre_auton(void) {
    Brain.Screen.clearScreen();
    Brain.Screen.drawImageFromFile("HOSLimited.png", 0, 0);

    hwInit();

    this_thread::sleep_for(1000);

    //vex::Gif gif("HOSAnimDemo.gif", 0, 0 );
    Brain.Screen.clearScreen();
    Brain.Screen.drawImageFromFile("HOSReady.png", 0, 0);
    dirDisplay.init();
}

int main() {
    // setup competition
    pre_auton();

    Competition.autonomous(autonomous);
    Competition.drivercontrol(userctl);

    //Competition.test_auton();

    //vex::Gif gif("HOSAnimDemo.gif", 0, 0 );

    while(1) {
        wait(10, msec);
    }
}