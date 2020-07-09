#ifndef HEADER_TESTFUNC_HPP
#define HEADER_TESTFUNC_HPP

#include <cmath>

auto test_func = [](double x, double y) -> double {
    //return sin(4*x)*cos(4*y);
    //return x*x*y + y*y*x + x*y;
    return sin(x+y);
};

#endif // HEADER_TESTFUNC_HPP