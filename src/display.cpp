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
void DirectionDisplay::update(double actAngle, int actSpeed, double expAngle, int expSpeed) {
    
    // actual direction
    // Calculate the end coordinates of the line inside the circle
    double actRadians = (-actAngle + 90) * (3.14159265 / 180);
    // Adjust scaling factor from 0.5 to a value that matches the new radius
    int actLineLength = actSpeed * ((radius-5)*0.01);
    int actNewX = centerX + (actLineLength * cos(actRadians));
    int actNewY = centerY - (actLineLength * sin(actRadians)); // Y is inverted (screen coordinates)

    // expected direction
    // Calculate the end coordinates of the line inside the circle
    double expRadians = (-expAngle + 90) * (3.14159265 / 180);
    // Adjust scaling factor from 0.5 to a value that matches the new radius
    int expLineLength = expSpeed * ((radius-5)*0.01);
    int expNewX = centerX + (expLineLength * cos(expRadians));
    int expNewY = centerY - (expLineLength * sin(expRadians)); // Y is inverted (screen coordinates)

    // clear the previous line
    Brain.Screen.setPenColor(color::black);
    Brain.Screen.drawLine(centerX, centerY, actOldX, actOldY);
    Brain.Screen.drawLine(centerX, centerY, expOldX, expOldY);

    // draw the new line
    Brain.Screen.setPenColor(color::white);
    Brain.Screen.drawLine(centerX, centerY, actNewX, actNewY);
    Brain.Screen.setPenColor(color::blue);
    Brain.Screen.drawLine(centerX, centerY, expNewX, expNewY);

    // Update the old coordinates
    actOldX = actNewX;
    actOldY = actNewY;
    expOldX = expNewX;
    expOldY = expNewY;
}

// Initialize the display
void DirectionDisplay::init() {
    // Draw the circle
    Brain.Screen.setPenColor(color::white);
    Brain.Screen.drawCircle(centerX, centerY, radius);
}
