#include "Rectangle.h"
#include "Logger.h"
#include <assert.h>

double Rectangle::getArea()
{
    double leftline = _leftbottom.getDistance(_lefttop);
    Logger::getInfoLogger() << "leftline = " << leftline << "\n";
    double rightline = _righttop.getDistance(_rightbottom);
    Logger::getInfoLogger() << "rightline= " << rightline << "\n";
    double topline = _lefttop.getDistance(_righttop);
    Logger::getInfoLogger() << "topline = " << topline << "\n";
    double bottomline = _leftbottom.getDistance(_rightbottom);
    Logger::getInfoLogger() << "bottomline = " << bottomline << "\n";
    checkIfValid(leftline, rightline, topline, bottomline);
    return leftline * topline;
}

double Rectangle::getPerimeter()
{
    double leftline = _leftbottom.getDistance(_lefttop);
    Logger::getInfoLogger() << "leftline = " << leftline << "\n";
    double rightline = _righttop.getDistance(_rightbottom);
    Logger::getInfoLogger() << "rightline= " << rightline << "\n";
    double topline = _lefttop.getDistance(_righttop);
    Logger::getInfoLogger() << "topline = " << topline << "\n";
    double bottomline = _leftbottom.getDistance(_rightbottom);
    Logger::getInfoLogger() << "bottomline = " << bottomline << "\n";
    checkIfValid(leftline, rightline, topline, bottomline);
    return leftline + topline + rightline + bottomline;
}

void Rectangle::checkIfValid(double leftline, double righline, double topline, double bottomline)
{
    assert(leftline == righline);
    assert(topline == bottomline);
}
