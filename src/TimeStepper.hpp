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

class TimeStepper {
private:
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

    const AbstractBoundaryCondition &boundary_;

public:
    TimeStepper(const AbstractBoundaryCondition &boundary, double dx, double dy, double re, double gx, double gy, double rel_eps, double safety_tau);

    explicit TimeStepper(const AbstractBoundaryCondition &boundary, const Options &options);

    /** Calculae the F function used to calculate the next u_ij */
    double calculateF(const State &state, const Stencils &stencils, double dt, index_t i, index_t j) const;
    /** Calculate the G function used to calculate the next v_ij */
    double calculateG(const State &state, const Stencils &stencils, double dt, index_t i, index_t j) const;


    matrix_t next_step(const State& curr_step);
};


#endif //NSG_TIMESTEPPER_HPP
