#include "Stencils.hpp"

#include <iostream>
#include <cmath>

Stencils::Stencils(double dx, double dy, double gamma) :
dx_(dx), dy_(dy), gamma_(gamma)
{

}

void Stencils::setNewGamma(double gamma)
{
    gamma_ = gamma;
}

/**
 * [df/dx]_i_j = [ f_(i+1)_j - f_i_j ] / dx
 * 
 * [df/dy]_i_j = [ f_i_(j+1) - f_i_j ] / dy
 */
double Stencils::firstDerivF(const matrix_t &f, PARAM p, index_t i, index_t j)
{
    index_t new_i = (p == PARAM::X) ? i+1 : i;
    index_t new_j = (p == PARAM::Y) ? j+1 : j;
    double d = (p == PARAM::X) ? dx_ : dy_;

    return (f[new_i][new_j] - f[i][j]) / d;
}

/**
 * [d^2f/dx^2]_i_j = [ f_(i+1)_j - 2*f_i_j + f_(i-1)_j ] / (dx)^2
 * 
 * [d^2f/dy^2]_i_j = [ f_i_(j+1) - 2*f_i_j + f_i_(j-1) ] / (dy)^2
 */ 
double Stencils::secondDerivF(const matrix_t &f, PARAM p, index_t i, index_t j)
{
    index_t new_i_fward = (p == PARAM::X) ? i+1 : i; // forward
    index_t new_i_bward = (p == PARAM::X) ? i-1 : i; // backward

    index_t new_j_fward = (p == PARAM::Y) ? j+1 : j; // forward
    index_t new_j_bward = (p == PARAM::Y) ? j-1 : j; // backward

    double d = (p == PARAM::X) ? dx_ : dy_;


    double forward = f[new_i_fward][new_j_fward];
    double middle = f[i][j];
    double backward = f[new_i_bward][new_j_bward];

    return (forward - 2*middle + backward) / (d*d);
}

/**
 * [d(f^2)/dx]_i_j = [ ((f_i_j + f_(i+1)_j)/4)^2 - ((f_(i-1)_j + f_i_j)/2)^2 ] / dx
 *                  + gamma [ |f_i_j + f_(i+1)_j|/2 * (f_i_j - f_(i+1)_j)/2 
 *                  - |f_(i-1)_j + f_i_j|/2 * (f_(i-1)_j - f_i_j)/2 ] / dx
 * 
 * [d(f^2)/dy]_i_j = [ ((f_i_j + f_i_(j+1))/2)^2 - ((f_i_(j-1) + f_i_j)/2)^2 ] / dy
 *                  + gamma [ |f_i_j + f_i_(j+1)|/2 * (f_i_j - f_i_(j+1))/2
 *                  - |f_i_(j-1) + f_i_j|/2 * (f_i_(j-1) - f_i_j)/2 ] / dy
 */ 
double Stencils::firstDerivFSquared(const matrix_t &f, PARAM p, index_t i, index_t j)
{
    index_t new_i_fward = (p == PARAM::X) ? i+1 : i; // forward
    index_t new_i_bward = (p == PARAM::X) ? i-1 : i; // backward

    index_t new_j_fward = (p == PARAM::Y) ? j+1 : j; // forward
    index_t new_j_bward = (p == PARAM::Y) ? j-1 : j; // backward

    double d = (p == PARAM::X) ? dx_ : dy_;

    // the stencil has the following form:
    // [ term_1^2 - term_2^2 
    // - gamma [|term_1|(term_3) - |term_2|(term_4)] ] / 4d.
    double term_1 = f[i][j] + f[new_i_fward][new_j_fward];
    double term_2 = f[new_i_bward][new_j_bward] + f[i][j];
    double term_3 = f[i][j] - f[new_i_fward][new_j_fward];
    double term_4 = f[new_i_bward][new_j_bward] - f[i][j];

    return (pow(term_1, 2) - pow(term_2, 2) - gamma_*(std::abs(term_1)*term_3 - std::abs(term_2)*term_4)) / (4*d);
}

/**
 * [d(fg)/dx]_i_j = [ (f_i_j + f_i_(j+1))/2 * (g_i_j + g_(i+1)_j)/2
 *                  - (f_(i-1)_j + f_(i-1)_(j+1))/2 * (g_(i-1)_j + g_i_j)/2 ] / dx
 *                  + gamma * [ |f_i_j + f_i_(j+1)|/2 * (g_i_j - g_(i+1)_j)/2
 *                  - |f_(i-1)_j + f_(i-1)_(j+1)|/2 * (g_(i-1)_j - g_i_j)/2 ] / dx
 * 
 * [d(fg)/dy]_i_j = [ (g_i_j + g_(i+1)_j)/2 * (f_i_j + f_i_(j+1))/2
 *                  - (g_i_(j-1) + g_(i+1)_(j-1))/2 * (f_i_(j-1) + f_i_j)/2 ] / dy
 *                  + gamma * [ |g_i_j + g_(i+1)_j|/2 * (f_i_j - f_i_(j+1))/2
 *                  - |g_i_(j-1) + g_(i+1)_(j-1)|/2 * (f_i_(j-1) - f_i_j)/2 ] / dy
 */ 
double Stencils::firstDerivFG(const matrix_t &f, const matrix_t &g, PARAM p, index_t i, index_t j)
{
    // conditional parameter
    // so we can easily add or substract 1 or 0, depending on if we derive after x or y
    index_t cond_x = (p == PARAM::X) ? 1 : 0; // 1 if d/dx, 0 if d/dy
    index_t cond_y = (p == PARAM::Y) ? 1 : 0; // 0 if d/dx, 1 if d/dy

    double d = (p == PARAM::X) ? dx_ : dy_;

    // f and g switch places in the formulas, depending on if we derive for x or y
    // always use d/dx as "standard"
    const matrix_t &new_f = (p == PARAM::X) ? f : g;
    const matrix_t &new_g = (p == PARAM::X) ? g : f;

    // the stencil has the following form
    // [ (term_f_1)*(term_g_1) - (term_f_2)*(term_g_2)
    // - gamma[ |term_f_1|*(term_g_3) - |term_f_2|*(term_g_4) ] ] / 4d.
    double term_f_1 = new_f[i][j] + new_f[i + cond_y][j + cond_x];
    double term_f_2 = new_f[i - cond_x][j - cond_y] + new_f[i - cond_x + cond_y][j + cond_x - cond_y];
    double term_g_1 = new_g[i][j] + new_g[i + cond_x][j + cond_y];
    double term_g_2 = new_g[i - cond_x][j - cond_y] + new_g[i][j];
    double term_g_3 = new_g[i][j] - new_g[i + cond_x][j + cond_y];
    double term_g_4 = new_g[i - cond_x][j - cond_y] - new_g[i][j];

    return (term_f_1*term_g_1 - term_f_2*term_g_2 + gamma_*(std::abs(term_f_1)*term_g_3 - std::abs(term_f_2)*term_g_4)) / (4*d);
}