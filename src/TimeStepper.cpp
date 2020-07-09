//
// Created by janhb on 01.07.2020.
//

#include "TimeStepper.hpp"
#include "TestFunc.hpp"

#include <vector>
#include <iostream>
#include <cmath>

using PARAM = Stencils::PARAM;

matrix_t TimeStepper::calculateF(const State &state, const Stencils &stencils, double dt) const
{
    matrix_t F = matrix_t(state.getCellCountX(), std::vector<double>(state.getCellCountY()));
    const matrix_t &u = state.u;
    const matrix_t &v = state.v;

    // we will deal with the ghost cells later
    for (index_t i = 1; i < F.size() - 1; i++) {
        for (index_t j = 1; j < F[i].size() - 1; j++) {
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
            F[i][j] = u[i][j] + dt * dt_brace;
        }
    }

    // apply boundary
    boundary_.applyFBoundaries(F, state.u);

    return F;
}

matrix_t TimeStepper::calculateG(const State &state, const Stencils &stencils, double dt) const
{
    matrix_t G = matrix_t(state.getCellCountX(), std::vector<double>(state.getCellCountY()));
    const matrix_t &u = state.u;
    const matrix_t &v = state.v;

    // we will deal with the ghost cells later
    for (index_t i = 1; i < G.size() - 1; i++) {
        for (index_t j = 1; j < G[i].size() - 1; j++) {
            // The value of the brace which is multiplied with dt
            // 1/Re * ( d^2v/dx^2 + d^2u/dy^2)
            double dt_brace = 1/Re_ * (stencils.secondDerivF(v, PARAM::X, i, j) + stencils.secondDerivF(v, PARAM::Y, i, j))
                            // - d(uv)/dx
                            - stencils.firstDerivFG(u, v, PARAM::X, i, j)
                            // - dv^2/dy
                            - stencils.firstDerivFSquared(v, PARAM::Y, i, j)
                            // + gy (gravity term)
                            + gy_;

            // F_ij = v_ij + dt * [...]
            G[i][j] = v[i][j] + dt * dt_brace;
        }
    }

    // apply boundary
    boundary_.applyGBoundaries(G, state.v);

    return G;
}

matrix_t TimeStepper::calculateRHS(const State &state, const Stencils &stencils, const matrix_t &F, const matrix_t &G, double dt) const
{   
    // this way the ghost cells are just 0 which is alright
    matrix_t RHS = matrix_t(state.getCellCountX(), std::vector<double>(state.getCellCountY()));

    // remember we don't want ghost cells
    for (index_t i = 1; i < RHS.size(); i++) {
        for (index_t j = 1; j < RHS[i].size(); j++) {
            // F_i_j - F_(i-1)_j
            double temp_f = F[i][j] - F[i-1][j];
            // G_i_j - G_i_(j-1)
            double temp_g = G[i][j] - G[i][j-1];

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
            //RHS[i][j] = test_func(i*state.getDX(), j*state.getDY());
            //RHS[i][j] = 2*(i*dx_ + j*dy_);
            //RHS[i][j] = -state.p[i][j];
        }
    }

    return RHS;
}

State TimeStepper::step(const State& curr_step)
{
    double u_max = SOR::normMAX(curr_step.u);
    double v_max = SOR::normMAX(curr_step.v);

    double dt = calculateDt(u_max, v_max);

    if (dt < 1e-4) {
        std::cout << "Very small dt, we have u_max=" << u_max << " and v_max=" << v_max << std::endl;
    }

    double gamma = calculateGamma(u_max, v_max, dt); // gamma should be greater equal than RHS

    State next_step = State(curr_step.getCellCountX(), curr_step.getCellCountY(), curr_step.getDX(), curr_step.getDY(), curr_step.getTime() + dt);   
    SOR sor_solver = SOR(boundary_, curr_step.p, curr_step.getDX(), curr_step.getDY(), rel_eps_, SOR::NORM::L2);
    Stencils stencils = Stencils(dx_, dy_, gamma);

    // calculate F, G and RHS
    matrix_t F = calculateF(curr_step, stencils, dt);
    matrix_t G = calculateG(curr_step, stencils, dt);
    matrix_t RHS = calculateRHS(curr_step, stencils, F, G, dt);

    // test only with p
    next_step.p = sor_solver.newFieldTest(calculateRHS_test(curr_step, stencils, dt), omega_, SOR_max_it_);

    // new pressure
    //next_step.p = sor_solver.newField(RHS, omega_, SOR_max_it_);

    // new u and v velocity
    calculateUV(next_step, F, G, stencils, dt);

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

double TimeStepper::calculateDt(double u_max, double v_max)
{
    double opt_1 = Re_ / 2 / ( 1/(dx_*dx_) + 1/(dy_*dy_));
    double opt_2 = dx_ / u_max;
    double opt_3 = dy_ / v_max;

    return safety_tau_ * std::min(opt_1, std::min(opt_2, opt_3));
}

double TimeStepper::calculateGamma(double u_max, double v_max, double dt)
{
    return std::max(u_max*dt/dx_, v_max*dt/dy_);
}

void TimeStepper::calculateUV(State &next_step, const matrix_t &F, const matrix_t &G, const Stencils &stencils, double dt)
{
    // deal with the ghost cells later
    for (index_t i = 1; i < next_step.getCellCountX() - 1; i++) {
        for (index_t j = 1; j < next_step.getCellCountY() - 1; j++) {
            next_step.u[i][j] = F[i][j] - dt*stencils.firstDerivF(next_step.p, Stencils::PARAM::X, i, j);
            next_step.v[i][j] = G[i][j] - dt*stencils.firstDerivF(next_step.p, Stencils::PARAM::Y, i, j);
        }
    }

    // apply boundaries
    boundary_.applyUBoundaries(next_step.u);
    boundary_.applyVBoundaries(next_step.v);
}