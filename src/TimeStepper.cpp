//
// Created by janhb on 01.07.2020.
//

#include "TimeStepper.hpp"

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

TimeStepper::TimeStepper(double dx, double dy, double re, double gx, double gy) :
dx_(dx), dy_(dy), Re_(re), gx_(gx), gy_(gy)
{
}

TimeStepper::TimeStepper(const Options &options)
{
    dx_ = options.getDx();
    dy_ = options.getDy();

    Re_ = options.getReynold();

    //TODO: Implement gravity.
    gx_ = 0;
    gy_ = 0;
}
