#include "vex.h"
#include "gifclass.h"
#include "display.h"
#include <algorithm>

using namespace vex;

// cycle time monitoring
float cycleTime = 0;
float cyclePerSecond = 0;
double cycleBegin = 0;

DirectionDisplay dirDisplay(150, 120, 80);

// Display update
void displayUpdate(double actualAngle, int actualSpeed, double expectedAngle, int expectedSpeed) {
    // cps
    Brain.Screen.printAt(1, 40, "T/%0.2f ms", cycleTime);
    Brain.Screen.printAt(1, 20, "C/%0.2f", cyclePerSecond);

    // direction display
    dirDisplay.update(actualAngle, actualSpeed);
}

int main() {
    hwInit();

    //vex::Gif gif("HOSAnimDemo.gif", 0, 0 );
    Brain.Screen.clearScreen();
    Brain.Screen.drawImageFromFile("HOSReady.png", 0, 0);
    dirDisplay.init();

    while(1) {
        cycleBegin = vex::timer::system();

        // actual code
        // Get acceleration in m/s^2 on the X and Y axes
        double accX = InertialSensor.acceleration(xaxis);
        double accY = InertialSensor.acceleration(yaxis);

        // Calculate the magnitude of the acceleration vector
        double accelerationMagnitude = sqrt(accX * accX + accY * accY);

        // Calculate the angle of the acceleration vector in degrees
        double accelerationAngle = atan2(accY, accX) * (180 / 3.14159265);

        // Display the acceleration magnitude and angle on the Brain's screen
        Brain.Screen.setCursor(20, 1);
        Brain.Screen.print("Accel Mag: %.2f m/s^2, Angle: %.2f degrees", accelerationMagnitude, accelerationAngle);

        // Get angle and magnitude from the controller's joysticks
        double expAngle = atan2(Controller1.Axis3.position(), Controller1.Axis1.position()) * (180 / 3.14159265);
        int expSpeed = sqrt(pow(Controller1.Axis1.position(), 2) + pow(Controller1.Axis3.position(), 2));

        // Limit speed to 100 (max value for display purposes)
        expSpeed = std::min(expSpeed, 100);

        displayUpdate(0, 100, expAngle, expSpeed);
        this_thread::sleep_for(5);

        cycleTime = vex::timer::system() - cycleBegin;
        cyclePerSecond = 1000 / cycleTime;
    }
}