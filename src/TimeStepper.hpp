//
// Created by janhb on 01.07.2020.
//

#ifndef NSG_TIMESTEPPER_HPP
#define NSG_TIMESTEPPER_HPP

#include "Common.hpp"
#include "Stencils.hpp"
#include "State.hpp"
#include "Options.hpp"
#include "AbstractBoundaryCondidtion.hpp"
#include "SOR.hpp"

class TimeStepper {
private:
    const AbstractBoundaryCondition &boundary_;

    double dx_;
    double dy_;

    /** Reynolds Zahl **/
    double Re_;

    /** Gravity in x direction */
    double gx_;
    /** Gravity in y direction */
    double gy_;

    // relative epsilon for residual
    double rel_eps_;

    // safety tau for choosing the next time step
    double safety_tau_;

    // omega for SOR Solver
    double omega_;
    
    // Iteration cap for SOR Solver
    index_t SOR_max_it_;

public:
    TimeStepper(const AbstractBoundaryCondition &boundary, double dx, double dy, double re, double gx, double gy, double rel_eps, double safety_tau, double omega, index_t SOR_max_it);

    explicit TimeStepper(const AbstractBoundaryCondition &boundary, const Options &options);

    State step(const State& curr_step);

private:
    /** Calculae the F function used to calculate the next u_ij */
    matrix_t calculateF(const State &state, const Stencils &stencils, double dt) const;
    /** Calculate the G function used to calculate the next v_ij */
    matrix_t calculateG(const State &state, const Stencils &stencils, double dt) const;

    /** Calculate RHS matrix, found in equation (41) in the instructions */
    matrix_t calculateRHS(const State &state, const Stencils &stencils, const matrix_t &F, const matrix_t &G, double dt) const;

    matrix_t calculateRHS_test(const State &state, const Stencils &stencils, double dt) const;

    double calculateDt(double u_max, double v_max);

    double calculateGamma(double u_max, double v_max, double dt);

    void calculateUV(State &next_step, const matrix_t &F, const matrix_t &G, const Stencils &stencils, double dt);
};


#endif //NSG_TIMESTEPPER_HPP
