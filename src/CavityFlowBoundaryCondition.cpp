//
// Created by janhb on 03.07.2020.
//

#include "CavityFlowBoundaryCondition.hpp"
#include <iostream>
#include <cmath>

#include "TestFunc.hpp"

void CavityFlowBoundaryCondition::applyPBoundaries(matrix_t &p) const {
    index_t width = p.size();
    index_t height = p[0].size();

    // von Neumann for actual fluid simulation
    // Dirichlet (von_neumann == false) for testing
    bool von_neumann = true;

    //The manual does not give a proper equation for how to fill the corner ghost cells
    //They are not accessed, so just fill it with its neighbor ghost value

    //Fill ghost cells in row
    for (index_t j=1; j < height-1; j++) {
        if (von_neumann) {
            p[0][j] = p[1][j];
            p[width - 1][j] = p[width - 2][j];
        }
        else {
            p[0][j] = test_func(0, j*options_.getDy());
            p[width - 1][j] = test_func((width - 1) * options_.getDx(), j*options_.getDy());
        }
    }

    //Fill column ghost cells
    for (index_t i=1; i < width-1; i++) {
        if (von_neumann) {
            p[i][0] = p[i][1];
            p[i][height - 1] = p[i][height - 2];
        }
        else {
            p[i][0] = test_func(i*options_.getDx(), 0);
            p[i][height-1] = test_func(i*options_.getDx(), (height-1)*options_.getDy());
        }
    }
}

void CavityFlowBoundaryCondition::applyUBoundaries(matrix_t &u) const {
    auto width = u[0].size();
    auto height = u.size();

    //Set no slip condition for u
    //We have to iterate until u.size() - 1 because last index is a ghost cell
    for (index_t j = 1; j < width - 1; j++) {
        u[0][j] = 0;

        //Set top row to velocity
        u[height - 1][j] = velocity_;
    }

    for (index_t i = 1; i < height - 1; i++) {
        u[i][0] = - u[i][1];
        u[i][width - 1] = u[i][width - 2];
    }
}

void CavityFlowBoundaryCondition::applyVBoundaries(matrix_t &v) const {
    auto height = v.size();
    auto width = v[0].size();

    //Set no slip condition for u
    //We have to iterate until u.size() - 1 because last index is a ghost cell
    for (index_t i = 1; i < height - 1; i++) {
        v[i][width] = 0;
        v[i][width - 1] = 0;
    }

    for (index_t j = 1; j < width - 1; j++) {
        v[0][j] = - v[1][j];
        v[height - 1][j] = - v[height - 2][j];
    }
}

CavityFlowBoundaryCondition::CavityFlowBoundaryCondition(const Options &options) :
options_(options)
{
    velocity_ = 0;
}
