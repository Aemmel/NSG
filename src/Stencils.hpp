#ifndef NSG_STENCILS_HEADER
#define NSG_STENCILS_HEADER

#include <cinttypes>
#include <functional>

#include "Common.hpp"

class Stencils {
public:
    enum class PARAM { X, Y };

private:
    double dx_;
    double dy_;
    double gamma_;

public:
    Stencils(double dx, double dy, double gamma);
    
    void setNewGamma(double gamma);

    double firstDerivF(const matrix_t &f, PARAM p, index_t i, index_t j);
    double secondDerivF(const matrix_t &f, PARAM p, index_t i, index_t j);
    double firstDerivFSquared(const matrix_t &f, PARAM p, index_t i, index_t j);
    double firstDerivFG(const matrix_t &f, const matrix_t &g, PARAM p, index_t i, index_t j);
};

#endif // NSG_STENICLS_HEADER