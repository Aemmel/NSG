//
// Created by janhb on 08.07.2020.
//

#include "AbstractBoundaryCondition.hpp"

void AbstractBoundaryCondition::applyGBoundaries(matrix_t &g, const matrix_t &v) const 
{
    index_t width = g.size();
    index_t height = g[0].size();

    //Apply boundary conditions
    for (index_t i = 1; i < width - 1; i++) {
        g[i][0] = v[i][0];
        g[i][height - 2] = v[i][height - 2];
    }
}

void AbstractBoundaryCondition::applyFBoundaries(matrix_t &f, const matrix_t &u) const
{
    index_t width = f.size();
    index_t height = f[0].size();

    //Apply boundary conditions
    for (index_t j = 1; j < height - 1 ; j++) {
        f[0][j] = u[0][j];
        f[width - 2][j] = u[width - 2][j];
    }
}