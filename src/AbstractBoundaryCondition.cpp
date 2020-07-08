//
// Created by janhb on 08.07.2020.
//

#include "AbstractBoundaryCondition.hpp"

matrix_t AbstractBoundaryCondition::applyGBoundaries(const matrix_t &g, const matrix_t &v) const {

    auto copy = matrix_t(g);

    index_t height = copy[0].size();

    //Apply boundary conditions
    for (index_t i = 1; i < height - 1; i++)
    {
        copy[i][0] = v[i][0];
        copy[i][height - 2] = v[i][height - 2];
    }

    return copy;
}

matrix_t AbstractBoundaryCondition::applyFBoundaries(const matrix_t &f, const matrix_t &u) const {
    auto copy = matrix_t(f);

    index_t width = copy.size();

    //Apply boundary conditions
    for (index_t j = 1; j < width - 1 ; j++) {
        copy[0][j] = u[0][j];
        copy[0][width - 2] = u[0][width - 2];
    }

    return copy;
}