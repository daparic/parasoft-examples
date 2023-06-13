#ifndef SHAPES_HPP_
#define SHAPES_HPP_

#include "Point.hpp"

class Shape {
	Point _position;
public:
    virtual double getArea() const = 0;
    Point& getPosition() { return _position; }
protected:
    Shape(Point position) : _position(position) {}
};

class LineSegment : public Shape {
	Point _end;
public:
    LineSegment(Point position, Point end) : Shape(position), _end(end) {}
    double getArea() const { return 0; }
};

class Circle : public Shape {
	double _radius;
public:
    Circle(Point position, double radius) : Shape(position), _radius(radius) {}
    double getArea() const { return 3.141592653 * _radius * _radius; }
    int contains(Point point)
    {
    	return point.squareDistanceTo(getPosition()) <= (_radius * _radius);
    }
};

#endif /*SHAPES_HPP_*/
