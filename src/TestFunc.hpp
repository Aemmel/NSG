#ifndef HEADER_TESTFUNC_HPP
#define HEADER_TESTFUNC_HPP

#include <cmath>

auto test_func = [](double x, double y) -> double {
    return sin(x)*cos(y);
    //return x*x*y + y*y*x + x*y;
    //return sin(x);
};

#endif // HEADER_TESTFUNC_HPP