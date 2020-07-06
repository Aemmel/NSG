#include "SOR.hpp"

#include <functional>
#include <cmath>
#include <iostream>

SOR::SOR(const AbstractBoundaryCondition &boundary, const matrix_t &orig_field, double dx, double dy, double rel_eps, NORM norm) :
boundary_(boundary), orig_field_(orig_field), dx_(dx), dy_(dy), rel_eps_(rel_eps), norm_(norm)
{
    // mostly to fix the size
    curr_field_ = matrix_t(orig_field); 
    next_field_ = matrix_t(orig_field);
}

matrix_t SOR::newField(const matrix_t &RHS, double omega, index_t max_it)
{
    matrix_t res;

    index_t cnt = 0;

    // main loop to calculte the new field
    do {
        // calculate new field
        newIteration(RHS, omega);

        curr_field_ = next_field_;

        // calculate residual
        res = calculateRes(next_field_, RHS);

        cnt++;
    } while(!resBelowError(res) && cnt < max_it);

    // inform not only how long it took, but also if it converged quickly enough
    std::cout << "SOR Solver took " << cnt << " iterations";
    if (cnt == max_it) {
        std::cout << ". Maximum was reached";
    }
    std::cout << std::endl;

    return curr_field_;
}

matrix_t SOR::newFieldTest(const matrix_t &RHS, double omega, index_t max_it)
{
    matrix_t res;
    matrix_t rhs = matrix_t(RHS);

    index_t cnt = 0;

    // main loop to calculte the new field
    do {
        // calculate new field
        newIteration(RHS, omega);

        curr_field_ = next_field_;

        // calculate residual
        res = calculateRes(next_field_, RHS);

        // calculate new RHS
        for (index_t i = 0; i < RHS.size(); i++) {
            for (index_t j = 0; j < RHS[i].size(); j++) {
                rhs[i][j] = - 2 * next_field_[i][j];
                //rhs[i][j] = - next_field_[i][j];
            }
        }

        cnt++;
    } while(!resBelowError(res) && cnt < max_it);

    // inform not only how long it took, but also if it converged quickly enough
    std::cout << "SOR Solver took " << cnt << " iterations";
    if (cnt == max_it) {
        std::cout << ". Maximum was reached";
    }
    std::cout << std::endl;

    return curr_field_;
}

void SOR::newIteration(const matrix_t &RHS, double omega)
{
    next_field_ = boundary_.applyPBoundaries(curr_field_);

    // remember we don't want the ghost cells
    for (index_t i = 1; i < next_field_.size() - 1; i++) {
        for (index_t j = 1; j < next_field_[i].size() - 1; j++) {
            // we break the term up into:
            // (1-w)p_i_j + fac * (temp_x / dx^2 + temp_y / dy^2 - RHS_i_j)
            double fac = omega / (2.0 * (1.0/(dx_*dx_) + 1.0/(dy_*dy_)));
            double temp_x = curr_field_[i+1][j] + next_field_[i-1][j];
            double temp_y = curr_field_[i][j+1] + next_field_[i][j-1];

            next_field_[i][j] = (1-omega)*curr_field_[i][j] + fac*(temp_x / (dx_ * dx_) + temp_y / (dy_ * dy_) - RHS[i][j]);
        }
    }
}

matrix_t SOR::calculateRes(const matrix_t &new_field, const matrix_t &RHS) const
{
    // we trust the vector sizes are all compatible
    matrix_t res = matrix_t(new_field.size(), std::vector<double>(new_field[0].size()));

    // remember we don't want the ghost cells
    for (index_t i = 1; i < res.size() - 1; i++) {
        for (index_t j = 1; j < res[i].size() - 1; j++) {
            // p_(i+1)_j - 2p_i_j + p_(i-1)_j
            double temp_x = new_field[i+1][j] - 2*new_field[i][j] + new_field[i-1][j];
            // p_i_(j+1) - 2p_i_j + p_i_(j-1)
            double temp_y = new_field[i][j+1] - 2*new_field[i][j] + new_field[i][j-1];

            res[i][j] = temp_x / (dx_*dx_) + temp_y / (dy_*dy_) - RHS[i][j];
        }
    }

    return res;
}

bool SOR::resBelowError(const matrix_t &res)
{
    std::function<double(const matrix_t&)> norm_func;

    switch (norm_) {
    case NORM::L2:
        norm_func = std::bind(&SOR::normL2, this, std::placeholders::_1);
        break;
    case NORM::MAX:
        norm_func = std::bind(&SOR::normMAX, this, std::placeholders::_1);
        break;
    default:
        throw std::runtime_error("Non-existent norm used!");
    }

    return norm_func(res) < rel_eps_ * norm_func(orig_field_);
}

/**
 * ||mat||_2 = (1/i_max/j_max sum_i=1^(i_max) sum_j=1^(j_max) (mat_i_j)^2)^(1/2)
 */ 
double SOR::normL2(const matrix_t &mat) const
{
    double temp = 0;
    index_t i_max = mat.size() - 2;
    index_t j_max = mat[0].size() - 2; // we assume rectangular size

    // remember we don't want the ghost cells
    for (index_t i = 1; i < mat.size() - 1; i++) {
        for (index_t j = 1; j < mat[i].size() - 1; j++) {
            temp += std::pow(mat[i][j], 2);
        }
    }

    return std::sqrt(temp / i_max / j_max);
}

/**
 * ||mat||_\infty = max_(i,j) (|mat_i_j|)
 */ 
double SOR::normMAX(const matrix_t &mat) const
{
    double max = 0;

    // remember we don't want the ghost cells
    for (index_t i = 1; i < mat.size() - 1; i++) {
        for (index_t j = 1; j < mat[i].size() - 1; j++) {
            if (std::abs(mat[i][j]) > max) {
                max = std::abs(mat[i][j]);
            }
        }
    }

    return max;
}