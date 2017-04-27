#ifndef RANDOM_H
#define RANDOM_H

// Based on : "Random Number Generation in C++11"
// (Document #: WG21 N3551)

#include <random>

// URNG (Uniform Random Generator) engine
std::default_random_engine& global_urng();

void randomize();

template<typename IntType>
IntType pick_an_integer(IntType from, IntType thru)
{
    static std::uniform_int_distribution<IntType>  d{};
    using  parm_t  = typename decltype(d)::param_type;
    return d( global_urng(), parm_t{from, thru} );
}

#endif // RANDOM_H
