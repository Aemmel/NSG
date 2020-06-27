//
// Created by janhb on 26.06.2020.
//

#ifndef NSG_ABSTRACTPRINTER_HPP
#define NSG_ABSTRACTPRINTER_HPP

#include "State.hpp"

class AbstractPrinter {
    virtual void print(State state, double time) = 0;
};


#endif //NSG_ABSTRACTPRINTER_HPP
