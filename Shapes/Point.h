#ifndef POINT_H_
#define POINT_H_

class Point {
public:
	Point(double x, double y) : _x(x), _y(y)
	{
	}

	double getX()
	{
		return _x;
	}

	double getY()
	{
		return _y;
	}

	double getDistance(Point p);

private:
	double _x;
	double _y;
};

#endif /* POINT_H_ */
