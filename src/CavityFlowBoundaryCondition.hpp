//
// Created by janhb on 03.07.2020.
//

#ifndef NSG_CAVITYFLOWBOUNDARYCONDITION_HPP
#define NSG_CAVITYFLOWBOUNDARYCONDITION_HPP


#include "Options.hpp"
#include "AbstractBoundaryCondition.hpp"

class CavityFlowBoundaryCondition : public AbstractBoundaryCondition {

private:
    double velocity_ = 0;

    Options options_;
public:
    explicit CavityFlowBoundaryCondition(const Options &options);



    void applyPBoundaries(matrix_t &p) const override;
    void applyUBoundaries(matrix_t &u, double t) const override ;
    void applyVBoundaries(matrix_t &v, double t) const override ;

};


#endif //NSG_CAVITYFLOWBOUNDARYCONDITION_HPP
