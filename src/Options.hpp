#ifndef NSG_OPTIONS_HPP
#define NSG_OPTIONS_HPP

#include <string>
#include <cinttypes>

class Options
{
private:
    // grid size
    uint_fast32_t cell_cnt_x_;
    uint_fast32_t cell_cnt_y_;
    
    // step size
    double dx_;
    double dy_;

    double reynold_;

    // relative epsilon for residual
    double rel_epsilon_;

    double max_time_;

public:
    Options(const std::string& file_name);

    uint_fast32_t getCellCntX();
    uint_fast32_t getCellCntY();
    
    double getDx();
    double getDy();

    double getReynold();

    double getRelEps();

    double getMaxTime();
};

#endif // NSG_OPTIONS_HPP