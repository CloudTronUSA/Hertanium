#include "display.h"
#include <cmath> // For cos and sin functions
#include <algorithm>

// Constructor to initialize the display properties and controller
DirectionDisplay::DirectionDisplay(int x, int y, int r)
: centerX(x), centerY(y), radius(r) {
    // Initial drawing of the circle
    Brain.Screen.setPenColor(color::white);
    Brain.Screen.drawCircle(centerX, centerY, radius);
}

// Update the direction and magnitude based on controller input
void DirectionDisplay::update(double angle, int speed) {
    // Calculate the end coordinates of the line inside the circle
    double radians = angle * (3.14159265 / 180);
    // Adjust scaling factor from 0.5 to a value that matches the new radius
    int lineLength = speed * ((radius-5)*0.01); // Adjust this factor so that speed of 100 roughly reaches the circle edge
    int newX = centerX + (lineLength * cos(radians));
    int newY = centerY - (lineLength * sin(radians)); // Y is inverted (screen coordinates)

    // clear the previous line
    Brain.Screen.setPenColor(color::black);
    Brain.Screen.drawLine(centerX, centerY, oldX, oldY);

    // draw the new line
    Brain.Screen.setPenColor(color::white);
    Brain.Screen.drawLine(centerX, centerY, newX, newY);

    oldX = newX;
    oldY = newY;
}

// Initialize the display
void DirectionDisplay::init() {
    // Draw the circle
    Brain.Screen.setPenColor(color::white);
    Brain.Screen.drawCircle(centerX, centerY, radius);
}
