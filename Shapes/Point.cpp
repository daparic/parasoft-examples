#include "Point.h"
#include "Logger.h"
#include "Point.h"
#include <math.h>

double Point::getDistance(Point p)
{
    double xlen = 0.0;
    Logger::getInfoLogger() << "computing xlen" << " (Point::getDistance())" << "\n";
    if (p.getX() > getX()) {
        xlen = p.getX() - getX();
        Logger::getInfoLogger() << "xlen=" << xlen << " (Point::getDistance())" << "\n";
    } else {
        xlen = getX() - p.getX();
        Logger::getInfoLogger() << "xlen=" << xlen << " (Point::getDistance())" << "\n";
    }
    double ylen = 0.0;
    Logger::getInfoLogger() << "computing ylen" << " (Point::getDistance())" << "\n";
    if (p.getY() > getY()) {
        ylen = p.getY() - getY();
        Logger::getInfoLogger() << "ylen=" << ylen << " (Point::getDistance())" << "\n";
    } else {
        ylen = getY() - p.getY();
        Logger::getInfoLogger() << "ylen=" << ylen << " (Point::getDistance())" << "\n";
    }
    double distance = sqrt((xlen * xlen) + (ylen * ylen));
    Logger::getInfoLogger() << "distance=" << distance << " (Point::getDistance())" << "\n";
    return distance;
}
