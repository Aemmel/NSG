//
// Created by janhb on 26.06.2020.
//

#ifndef NSG_STATE_HPP
#define NSG_STATE_HPP

#include <cinttypes>
#include <vector>

/**
 * Instances of this classes contains the current state of the system.
 * It contains the (2D) velocity field and the (1D) pressure field, on a grid.
 */
class State {

private:
    uint_fast32_t width_;
    uint_fast32_t height_;

    double dx_;
    double dy_;

public:


    /**
     * The x component of the velocity
     */
    std::vector<std::vector<double>> u;

    /**
     * The y component of the velocity;
     */
    std::vector<std::vector<double>> v;

    /**
     * T
     */
    std::vector<std::vector<double>> p;

    /**
     * Creates a new State object
     * @param width The number of cells in x direction
     * @param height The number of cells in y direction
     * @param dx The distance between two cells in x direction
     * @param dy The distance used between two cells in y direction
     */
    State(uint_fast32_t width, uint_fast32_t height, double dx, double dy);

    /**
     * Gets the number of cells in X direction
     * @return
     */
    uint_fast32_t getCellCountX() const;

    /**
     * Gets the number of cells in Y direction
     * @return
     */
    uint_fast32_t getCellCountY() const;

    /**
     * Returns the distance between two cells in x direction
     * @return
     */
    double getDX() const;

    /**
     * Returns the distance between two cells in y direction
     * @return
     */
    double getDY() const;

    /**
     * Returns the width of the grid (in the same unit as dx)
     * @return
     */
    double getWidth() const;

    /**
     * Returns the height of the grid (in the same unit as dy)
     * @return
     */
    double getHeight() const;
};


#endif //NSG_STATE_HPP
