//
// Created by janhb on 03.07.2020.
//

#include "CavityFlowBoundaryCondition.hpp"
#include <iostream>
#include <cmath>

matrix_t CavityFlowBoundaryCondition::applyPBoundaries(const matrix_t &p) const {
    auto copy = matrix_t(p);

    index_t width = p.size();
    index_t height = p[0].size();

    // von Neumann for actual fluid simulation
    // Dirichlet (von_neumann == false) for testing
    bool von_neumann = false;

    //The manual does not give a proper equation for how to fill the corner ghost cells
    //They are not accessed, so just fill it with its neighbor ghost value

    //Fill ghost cells in row
    for (index_t j=1; j < height-1; j++) {
        if (von_neumann == true) {
            copy[0][j] = p[1][j];
            copy[width - 1][j] = p[width - 2][j];
        }
        else {
            copy[0][j] = 0;
            copy[width - 1][j] = sin((width-1)*options_.getDx())*cos(j*options_.getDy());
        }
    }

    //Fill column ghost cells
    for (index_t i=1; i < width-1; i++) {
        if (von_neumann == true) {
            copy[i][0] = p[i][1];
            copy[i][height - 1] = p[i][height - 2];
        }
        else {
            copy[i][0] = sin(i*options_.getDx());
            copy[i][height-1] = sin(i*options_.getDx())*cos((height-1)*options_.getDy());
        }
    }

    return copy;
}

matrix_t CavityFlowBoundaryCondition::applyUBoundaries(const matrix_t &u) const {
    auto copy = matrix_t(u);

    auto width = copy[0].size();
    auto height = copy.size();

    //Set no slip condition for u
    //We have to iterate until u.size() - 1 because last index is a ghost cell
    for (index_t j = 1; j < width - 1; j++) {
        copy[0][j] = 0;

        //Set top row to velocity
        copy[height - 1][j] = velocity_;
    }

    for (index_t i = 1; i < height - 1; i++) {
        copy[i][0] = - copy[i][1];
        copy[i][width - 1] = copy[i][width - 2];
    }


    return copy;
}

matrix_t CavityFlowBoundaryCondition::applyVBoundaries(const matrix_t &v) const {
    auto copy = matrix_t(v);

    auto height = copy.size();
    auto width = copy[0].size();

    //Set no slip condition for u
    //We have to iterate until u.size() - 1 because last index is a ghost cell
    for (index_t i = 1; i < height - 1; i++) {
        copy[i][width] = 0;
        copy[i][width - 1] = 0;
    }

    for (index_t j = 1; j < width - 1; j++) {
        copy[0][j] = - v[1][j];
        copy[height - 1][j] = - v[height - 2][j];
    }

    return copy;
}

CavityFlowBoundaryCondition::CavityFlowBoundaryCondition(const Options &options) :
options_(options)
{
    velocity_ = 0;
}
