#include "Stencils.hpp"

Stencils::Stencils(double dx, double dy, double gamma) :
dx_(dx), dy_(dy), gamma_(gamma)
{

}

/**
 * [df/dx]_i_j = [ f_(i+1)_j - f_i_j ] / dx
 * [df/dy]_i_j = [ f_i_(j+1) - f_i_j ] / dy
 */
double Stencils::firstDerivF(const matrix_t& f, PARAM p, index_t i, index_t j)
{
    index_t new_i = (p == PARAM::X) ? i+1 : i;
    index_t new_j = (p == PARAM::Y) ? j+1 : j;
    double d = (p == PARAM::X) ? dx_ : dy_;


    return (f[new_i][new_j] - f[i][j]) / d;
}

/**
 * [d^2f/dx^2]_i_j = [ f_(i+1)_j - 2*f_i_j + f_(i-1)_j ] / (dx)^2
 * [d^2f/dy^2]_i_j = [ f_i_(j+1) - 2*f_i_j + f_i_(j-1) ] / (dy)^2
 */ 
double Stencils::secondDerivF(const matrix_t& f, PARAM p, index_t i, index_t j)
{
    index_t new_i_fward = (p == PARAM::X) ? i+1 : i; // forward
    index_t new_i_bward = (p == PARAM::X) ? i-1 : i; // backward

    index_t new_j_fward = (p == PARAM::Y) ? j+1 : j; // forward
    index_t new_j_bward = (p == PARAM::X) ? j-1 : j; // backward

    double d = (p == PARAM::X) ? dx_ : dy_;


    double forward = f[new_i_fward][new_j_fward];
    double middle = f[i][j];
    double backward = f[new_i_bward][new_j_bward];

    return (forward - 2*middle + backward) / (d*d);
}

double Stencils::firstDerivFSquared(const matrix_t& f, PARAM p, index_t i, index_t j)
{

}

double Stencils::firstDerivFG(const matrix_t& f, const matrix_t& g, PARAM p, index_t i, index_t j)
{

}

void Stencils::fillWithFunction(matrix_t& m, std::function<double(double, double)> func)
{
    for (index_t i = 0; i < m.size() ; i++) {
        for (index_t j = 0; j < m[i].size() ; j++) {
            m[i][j] = func(i * dx_, j * dy_);
        }
    }
}