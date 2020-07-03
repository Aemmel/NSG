//
// Created by janhb on 03.07.2020.
//

#ifndef NSG_ABSTRACTBOUNDARYCONDIDTION_HPP
#define NSG_ABSTRACTBOUNDARYCONDIDTION_HPP

#include "Common.hpp"

class AbstractBoundaryCondition
{
    virtual void applyPBoundaries(matrix_t &p) const = 0 ;
    virtual void applyUBoundaries(matrix_t &u) const = 0;
    virtual void applyVBoundaries(matrix_t &v) const = 0;
};


#endif //NSG_ABSTRACTBOUNDARYCONDIDTION_HPP
