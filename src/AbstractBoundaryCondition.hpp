//
// Created by janhb on 03.07.2020.
//

#ifndef NSG_ABSTRACTBOUNDARYCONDITION_HPP
#define NSG_ABSTRACTBOUNDARYCONDITION_HPP

#include "Common.hpp"

class AbstractBoundaryCondition
{
public:
    virtual matrix_t applyPBoundaries(const matrix_t &p) const = 0;
    virtual matrix_t applyUBoundaries(const matrix_t &u) const = 0;
    virtual matrix_t applyVBoundaries(const matrix_t &v) const = 0;

    matrix_t applyFBoundaries(const matrix_t &f, const matrix_t &u) const;
    matrix_t applyGBoundaries(const matrix_t &g, const matrix_t &v) const;
};

#endif //NSG_ABSTRACTBOUNDARYCONDITION_HPP
