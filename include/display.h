#ifndef DISPLAY_H
#define DISPLAY_H

#include "vex.h"

class DirectionDisplay {
private:
    int centerX;  // X-coordinate of the circle's center
    int centerY;  // Y-coordinate of the circle's center
    int radius;   // Radius of the circle
    int actOldX;     // X-coordinate of the previous line end
    int actOldY;     // Y-coordinate of the previous line end
    int expOldX;     // X-coordinate of the previous line end
    int expOldY;     // Y-coordinate of the previous line end

public:
    DirectionDisplay(int x, int y, int r);
    void update(double actAngle, int actSpeed, double expAngle, int expSpeed);
    void init();
};

#endif // DISPLAY_H
