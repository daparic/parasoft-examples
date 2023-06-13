#include "BetterRectangle.h"

double BetterRectangle::getArea()
{
    double area = _horlength * _verlength;
    return area;
}

double BetterRectangle::getPerimeter()
{
    double perimeter = (_horlength + _verlength) * 2;
    return perimeter;
}
