#include "random.h"

std::default_random_engine& global_urng()
{
    static std::default_random_engine  u{};
    return u;
}

void randomize()
{
    static std::random_device  rd{};
    global_urng().seed( rd() );
}
