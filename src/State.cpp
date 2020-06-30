//
// Created by janhb on 26.06.2020.
//

#include "State.hpp"
#include <cmath>
#include <iostream>

using std::vector;

State::State(index_t width, index_t height, double dx, double dy)
{
    width_ = width;
    height_ = height;
    dx_ = dx;
    dy_ = dy;

    //Initalize the size of the Arrays
    u = vector<vector<double>>(height, vector<double>(width));
    v = vector<vector<double>>(height, vector<double>(width));
    p = vector<vector<double>>(height, vector<double>(width));
}

index_t State::getCellCountX() const {
    return width_;
}

index_t State::getCellCountY() const {
    return height_;
}

double State::getDX() const {
    return dx_;
}

double State::getDY() const {
    return dy_;
}

double State::getWidth() const {
    return width_;
}

double State::getHeight() const {
    return height_;
}

void State::fillWithFunction(GRID grid, std::function<double(double, double)> func)
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

    for (index_t i = 0; i < this->height_ ; i++) {
        for (index_t j = 0; j < this->width_ ; j++) {
            array->at(i).at(j) = func(i * this->dx_, j * this->dy_);
        }
    }
}




