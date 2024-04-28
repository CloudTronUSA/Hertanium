#ifndef DISPLAY_H
#define DISPLAY_H

#include "vex.h"

class DirectionDisplay {
private:
    int centerX;  // X-coordinate of the circle's center
    int centerY;  // Y-coordinate of the circle's center
    int radius;   // Radius of the circle
    int oldX;     // X-coordinate of the previous line end
    int oldY;     // Y-coordinate of the previous line end

public:
    DirectionDisplay(int x, int y, int r);
    void update(double angle, int speed);
    void init();
};

#endif // DISPLAY_H
