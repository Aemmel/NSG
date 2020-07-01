#ifndef NSG_OPTIONS_HPP
#define NSG_OPTIONS_HPP

#include <string>
#include <cinttypes>

#include "Common.hpp"

class Options
{
private:
    // grid size
    index_t cell_cnt_x_;
    index_t cell_cnt_y_;
    
    // step size
    double dx_;
    double dy_;

    double reynold_;

    // relative epsilon for residual
    double rel_epsilon_;

    double max_time_;

public:
    Options(const std::string& file_name);

    index_t getCellCntX() const;
    index_t getCellCntY() const;
    
    double getDx() const;
    double getDy() const;

    double getReynold() const;

    double getRelEps() const;

    double getMaxTime() const;
};

#endif // NSG_OPTIONS_HPP