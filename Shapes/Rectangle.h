#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "Shape.h"
#include "Point.h"

class Rectangle : public Shape {
public:
    Rectangle(double lefttopx, double lefttopy,
              double leftbottomx, double leftbottomy,
              double righttopx, double righttopy,
              double rightbottomx, double rightbottomy) :
        _lefttop(Point(lefttopx, lefttopy)),
        _leftbottom(Point(leftbottomx, leftbottomy)),
        _righttop(Point(righttopx, righttopy)),
        _rightbottom(Point(rightbottomx, rightbottomy))
    {
    }

    virtual double getArea();
    virtual double getPerimeter();

    void checkIfValid(double leftline, double righline, double topline, double bottomline);

private:
    Point _lefttop;
    Point _leftbottom;
    Point _righttop;
    Point _rightbottom;
};

#endif /* RECTANGLE_H_ */
