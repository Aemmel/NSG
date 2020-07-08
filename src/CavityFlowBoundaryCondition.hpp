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



    matrix_t applyPBoundaries(const matrix_t &p) const override;
    matrix_t applyUBoundaries(const matrix_t &u) const override ;
    matrix_t applyVBoundaries(const matrix_t &v) const override ;

};


#endif //NSG_CAVITYFLOWBOUNDARYCONDITION_HPP
