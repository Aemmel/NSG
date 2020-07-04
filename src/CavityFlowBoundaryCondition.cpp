//
// Created by janhb on 03.07.2020.
//

#include "CavityFlowBoundaryCondition.hpp"


matrix_t CavityFlowBoundaryCondition::applyPBoundaries(const matrix_t &p) const {
    auto copy = matrix_t(p);

    index_t width = p[0].size();
    index_t height = p.size();

    //The manual does not give a proper equation for how to fill the corner ghost cells
    //They are not accessed, so just fill it with its neighbor ghost value

    //Fill ghost cells in row
    for (index_t j=0; j < width; j++) {
        copy[0][j] = p[1][j];
        copy[width - 1][j] = p[width - 2][j];
    }

    //Fill column ghost cells
    for (index_t i=0; i < height; i++) {
        copy[i][0] = p[i][1];
        copy[i][height - 1] = copy[i][height - 2];
    }

    return copy;
}

matrix_t CavityFlowBoundaryCondition::applyUBoundaries(const matrix_t &u) const {
    auto copy = matrix_t(u);

    return copy;
}

matrix_t CavityFlowBoundaryCondition::applyVBoundaries(const matrix_t &v) const {
    auto copy = matrix_t(v);

    return copy;
}

CavityFlowBoundaryCondition::CavityFlowBoundaryCondition(const Options &options)
{

}
