#ifndef POINT_HPP_
#define POINT_HPP_

class Point {
    int _x, _y;
public:
    Point(int x, int y) : _x(x), _y(y) {}
    void reflectAcrossX()
    {
        _y = -_y;
    }
    
    void translate(const Point& vector)
    {
    	_x += vector._x;
    	_y += vector._y;
    }
    
    int squareDistanceTo(const Point& point)
    {
    	return ((_x - point._x) * (_x - point._x)) + ((_y - point._y) * (_y - point._y));
    }
};

#endif /*POINT_HPP_*/
