#ifndef HEADER_SOR_HPP
#define HEADER_SOR_HPP

#include "AbstractBoundaryCondition.hpp"
#include "Common.hpp"

class SOR
{
public:
    enum class NORM { L2, MAX };

private:
    const AbstractBoundaryCondition &boundary_;
    
    // starting field, it = 0
    const matrix_t &orig_field_;
    matrix_t curr_field_;
    matrix_t next_field_;

    double norm_orig_field_;

    double dx_;
    double dy_;
    double rel_eps_;

    NORM norm_;

public:
    SOR(const AbstractBoundaryCondition &boundary, const matrix_t &orig_field, double dx, double dy, double rel_eps, NORM norm);

    matrix_t newField(const matrix_t &RHS, double omega, index_t max_it);
    matrix_t newFieldTest(const matrix_t &RHS, double omega, index_t max_it);

    static double normL2(const matrix_t &mat);
    static double normMAX(const matrix_t &mat);
private:
    //matrix_t newIteration(const matrix_t &curr_field, const matrix_t &RHS, double omega);
    void newIteration(const matrix_t &RHS, double omega);

    matrix_t calculateRes(const matrix_t &new_field, const matrix_t &RHS) const;

    bool resBelowError(const matrix_t &res);
};

#endif // HEADER_SOR_HPP