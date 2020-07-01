//
// Created by janhb on 01.07.2020.
//

#ifndef NSG_TIMESTEPPER_HPP
#define NSG_TIMESTEPPER_HPP

#include "Common.hpp"
#include "Stencils.hpp"
#include "State.hpp"
#include "Options.hpp"

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

public:
    TimeStepper(double dx, double dy, double re, double gx, double gy);

    explicit TimeStepper(const Options &options);

    /** Calculae the F function used to calculate the next u_ij */
    double calculateF(const State &state, const Stencils &stencils, double dt, index_t i, index_t j) const;
    /** Calculate the G function used to calculate the next v_ij */
    double calculateG(const State &state, const Stencils &stencils, double dt, index_t i, index_t j) const;

};


#endif //NSG_TIMESTEPPER_HPP
