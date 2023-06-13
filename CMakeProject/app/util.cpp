#include "mod1/mod1.h"
#include "mod3/mod3.h"

int util(int a)
{
    return mod1::mod1(a) + mod3::mod3(a);
}