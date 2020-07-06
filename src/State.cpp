//
// Created by janhb on 26.06.2020.
//

#include "State.hpp"
#include <cmath>
#include <iostream>

using std::vector;

State::State(index_t width, index_t height, double dx, double dy, double t)
{
    width_ = width;
    height_ = height;
    dx_ = dx;
    dy_ = dy;
    t_ = t;

    // Initalize the size of the Arrays
    // keep in mind: in the instructions the convention "Culumns first, then Rows" is used, so that
    // U_i_j is interpreted as i == x value and j == y value, unlike normal index notation
    // or how computers usually store 2D arrays. To compensate, we simply transpose the array in this step (width_ rows and height_ columns)
    // so that we can use the computer native notation from here on
    // though we ALWAYS have to remember, that we have width_ rows and height_ columns!!!!!
    u = matrix_t(width_, vector<double>(height_, 0));
    v = matrix_t(width_, vector<double>(height_, 0));
    p = matrix_t(width_, vector<double>(height_, 0));
}

index_t State::getCellCountX() const 
{
    return width_;
}

index_t State::getCellCountY() const 
{
    return height_;
}

double State::getDX() const 
{
    return dx_;
}

double State::getDY() const 
{
    return dy_;
}

double State::getWidth() const 
{
    return width_;
}

double State::getHeight() const 
{
    return height_;
}

double State::getTime() const
{
    return t_;
}

void State::fillWithFunction(GRID grid, const std::function<double(double, double)>& func)
{
    //vector<vector<double>> array;

    auto* array = &(this->u);

    switch(grid) {
        case GRID::U:
            array = &(this->u);
            break;
        case GRID::V:
            array = &(this->v);
            break;
        case GRID::P:
            array = &(this->p);
            break;
    }

    #pragma omp parallel for collapse(2)
    for (index_t i = 0; i < this->width_ ; i++) {
        for (index_t j = 0; j < this->height_ ; j++) {
            array->at(i).at(j) = func(i * this->dx_, j * this->dy_);
        }
    }
}

State::State(const Options &options, double t) :
 State(options.getCellCntX(), options.getCellCntY(), options.getDx(), options.getDy(), t)
{
    
}




