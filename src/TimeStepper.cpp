//
// Created by janhb on 01.07.2020.
//

#include "TimeStepper.hpp"
#include "SOR.hpp"

#include <vector>
#include <iostream>
#include <cmath>

using PARAM = Stencils::PARAM;

double TimeStepper::calculateF(const State &state, const Stencils &stencils, double dt, index_t i, index_t j) const
{
    const matrix_t &u = state.u;
    const matrix_t &v = state.v;

    //The value of the brace which is multiplied with dt
    // 1/Re * ( d^2u/dx^2 + d^2u/dy^2)
    double dt_brace = 1/Re_ * (stencils.secondDerivF(u, PARAM::X, i, j) + stencils.secondDerivF(u, PARAM::Y, i, j))
            // - du^2/dx
            - stencils.firstDerivFSquared(u, PARAM::X, i, j)
            // - d(uv)/dy
            - stencils.firstDerivFG(u, v, PARAM::Y, i, j)
            // + gx (gravity term)
            + gx_;

    // F_ij = u_ij + dt * [...]
    return u[i][j] + dt * dt_brace;
}

double TimeStepper::calculateG(const State &state, const Stencils &stencils, double dt, index_t i, index_t j) const
{
    const matrix_t &u = state.u;
    const matrix_t &v = state.v;

    //The value of the brace which is multiplied with dt
    // 1/Re * ( d^2v/dx^2 + d^2u/dy^2)
    double dt_brace = 1/Re_ * (stencils.secondDerivF(v, PARAM::X, i, j) + stencils.secondDerivF(v, PARAM::Y, i, j))
                      // - d(uv)/dx
                      - stencils.firstDerivFG(u, v, PARAM::X, i, j)
                      // - dv^2/dy
                      - stencils.firstDerivFSquared(v, PARAM::Y, i, j)
                      // + gy (gravity term)
                      + gy_;

    // F_ij = v_ij + dt * [...]
    return v[i][j] + dt * dt_brace;
}

matrix_t TimeStepper::calculateRHS(const State &state, const Stencils &stencils, double dt) const
{   
    matrix_t RHS = matrix_t(state.getCellCountX(), std::vector<double>(state.getCellCountY()));

    // remember we don't want ghost cells
    for (index_t i = 1; i < RHS.size(); i++) {
        for (index_t j = 1; j < RHS[i].size(); j++) {
            // F_i_j - F_(i-1)_j
            double temp_f = calculateF(state, stencils, dt, i, j) - calculateF(state, stencils, dt, i-1, j);
            // G_i_j - G_i_(j-1)
            double temp_g = calculateG(state, stencils, dt, i, j) - calculateG(state, stencils, dt, i, j-1);

            RHS[i][j] = ( temp_f / state.getDX() + temp_g / state.getDY() ) / dt;
        }
    }

    return RHS;
}

// test RHS, to simply solve
// Laplace p(x,y) = -2*p(x,y)
matrix_t TimeStepper::calculateRHS_test(const State &state, const Stencils &stencils, double dt) const
{
    matrix_t RHS = matrix_t(state.getCellCountX(), std::vector<double>(state.getCellCountY()));

    // remember we don't want ghost cells
    for (index_t i = 0; i < RHS.size(); i++) {
        for (index_t j = 0; j < RHS[i].size(); j++) {
            RHS[i][j] = - 2 * state.p[i][j];
        }
    }

    return RHS;
}

State TimeStepper::step(const State& curr_step)
{
    State next_step = State(curr_step.getCellCountX(), curr_step.getCellCountY(), curr_step.getDX(), curr_step.getDY(), curr_step.getTime());   
    SOR sor_solver = SOR(boundary_, curr_step.p, curr_step.getDX(), curr_step.getDY(), rel_eps_, SOR::NORM::L2);
    Stencils stencils = Stencils(curr_step.getDX(), curr_step.getDY(), 0.5);

    // test only with p
    next_step.p = sor_solver.newFieldTest(calculateRHS_test(curr_step, stencils, 0), omega_, SOR_max_it_);

    return next_step;
}

TimeStepper::TimeStepper(const AbstractBoundaryCondition &boundary, double dx, double dy, double re, double gx, double gy, double rel_eps, double safety_tau, double omega, index_t SOR_max_it) :
boundary_(boundary), dx_(dx), dy_(dy), Re_(re), gx_(gx), gy_(gy), rel_eps_(rel_eps), safety_tau_(safety_tau), omega_(omega), SOR_max_it_(SOR_max_it)
{
    
}

TimeStepper::TimeStepper(const AbstractBoundaryCondition &boundary, const Options &options): boundary_(boundary)
{
    dx_ = options.getDx();
    dy_ = options.getDy();

    Re_ = options.getReynold();

    //TODO: Implement gravity.
    gx_ = 0;
    gy_ = 0;

    rel_eps_ = options.getRelEps();

    safety_tau_ = options.getSafetyTau();

    omega_ = options.getOmega();

    SOR_max_it_ = options.getSORMaxIt();
}
