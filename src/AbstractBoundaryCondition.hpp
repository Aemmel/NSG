//
// Created by janhb on 03.07.2020.
//

#ifndef NSG_ABSTRACTBOUNDARYCONDITION_HPP
#define NSG_ABSTRACTBOUNDARYCONDITION_HPP

#include "Common.hpp"

class AbstractBoundaryCondition
{
public:
    virtual void applyPBoundaries(matrix_t &p) const = 0;
    virtual void applyUBoundaries(matrix_t &u, double t) const = 0;
    virtual void applyVBoundaries(matrix_t &v, double t) const = 0;

    void applyFBoundaries(matrix_t &f, const matrix_t &u) const;
    void applyGBoundaries(matrix_t &g, const matrix_t &v) const;
};

#endif //NSG_ABSTRACTBOUNDARYCONDITION_HPP
