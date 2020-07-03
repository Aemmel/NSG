//
// Created by janhb on 03.07.2020.
//

#ifndef NSG_ABSTRACTBOUNDARYCONDIDTION_HPP
#define NSG_ABSTRACTBOUNDARYCONDIDTION_HPP

#include "Common.hpp"

class AbstractBoundaryCondition
{
    virtual matrix_t applyPBoundaries(const matrix_t &p) const = 0;
    virtual matrix_t applyUBoundaries(const matrix_t &u) const = 0;
    virtual matrix_t applyVBoundaries(const matrix_t &v) const = 0;
};


#endif //NSG_ABSTRACTBOUNDARYCONDIDTION_HPP
