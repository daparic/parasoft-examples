#ifndef BETTERSQUARE_H_
#define BETTERSQUARE_H_

#include "BetterRectangle.h"

class BetterSquare : public BetterRectangle
{
public:
    BetterSquare(double lefttopx, double lefttopy, double linelength) :
        BetterRectangle(lefttopx, lefttopy, linelength, linelength)
    {
    }
};

#endif /* BETTERSQUARE_H_ */
