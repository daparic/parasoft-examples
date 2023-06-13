#include "mod3/mod3.h"

#include "mod1/mod1.h"
#include "mod2/mod2.h"

namespace mod3
{
    int mod3(int a)
    {
        return mod1::mod1(a) + mod2::mod2(a);
    }
}