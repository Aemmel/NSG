//
// Created by janhb on 03.07.2020.
//

#ifndef NSG_CAVITYFLOWBOUNDARYCONDITION_HPP
#define NSG_CAVITYFLOWBOUNDARYCONDITION_HPP


#include "Options.hpp"
#include "AbstractBoundaryCondidtion.hpp"

class CavityFlowBoundaryCondition : public AbstractBoundaryCondition {

private:

public:
    CavityFlowBoundaryCondition(const Options &options);

    CavityFlowBoundaryCondition();


    matrix_t applyPBoundaries(const matrix_t &p) const override;
    matrix_t applyUBoundaries(const matrix_t &u) const override ;
    matrix_t applyVBoundaries(const matrix_t &v) const override ;

};


#endif //NSG_CAVITYFLOWBOUNDARYCONDITION_HPP
