#include "Logger.h"
#include "Point.h"
#include "Square.h"
#include "Rectangle.h"
#include "BetterSquare.h"
#include "BetterRectangle.h"
#include "Logger.h"

int main()
{
    Logger::getWarnLogger().setEnabled(true);

    Square s(0, 10, 0, 0, 10, 10, 10, 0);
    Logger::getWarnLogger() << "** Square area " << s.getArea() << "\n";
    Logger::getWarnLogger() << "** Square perimeter " << s.getPerimeter() << "\n";

    Rectangle r(0, 20, 0, 0, 10, 20, 10, 0);
    Logger::getWarnLogger() << "** Rectangle area " << r.getArea() << "\n";
    Logger::getWarnLogger() << "** Rectangle perimeter " << r.getPerimeter() << "\n";

    BetterSquare bs(0, 10, 10);
    Logger::getWarnLogger() << "** BetterSquare area " << bs.getArea() << "\n";
    Logger::getWarnLogger() << "** BetterSquare perimeter " << bs.getPerimeter() << "\n";

    BetterRectangle br(0, 20, 10, 20);
    Logger::getWarnLogger() << "** BetterRectangle area " << br.getArea() << "\n";
    Logger::getWarnLogger() << "** BetterRectangle perimeter " << br.getPerimeter() << "\n";
    return 0;
}
