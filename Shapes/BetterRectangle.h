#ifndef BETTERRECTANGLE_H_
#define BETTERRECTANGLE_H_

#include "Shape.h"
#include "Point.h"

class BetterRectangle : public Shape
{
public:
    BetterRectangle(double lefttopx, double lefttopy, double horlength, double verlength) :
        _lefttop(Point(lefttopx, lefttopy)), _horlength(horlength), _verlength(verlength)
    {
    }

    virtual double getArea();
    virtual double getPerimeter();

private:
    Point _lefttop;
    double _horlength;
    double _verlength;
};

#endif /* BETTERRECTANGLE_H_ */
