#include "json.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>

#include "Options.hpp"

using json = nlohmann::json;
using std::cout;
using std::endl;

Options::Options(const std::string& file_name)
{
    std::ifstream json_file(file_name);

    if (json_file.is_open() == false) {
        throw std::runtime_error("Failed to open json_file");
    }

    json j;
    json_file >> j;

    cell_cnt_x_ = j["cell_cnt_x"];
    cell_cnt_y_ = j["cell_cnt_y"];

    if (cell_cnt_x_ < 3 || cell_cnt_y_ < 3) {
        throw std::runtime_error("cell_cnt_x/y needs to be 3 or larger!");
    }

    dx_ = j["dx"];
    dy_ = j["dy"];

    reynold_ = j["reynold"];

    rel_epsilon_ = j["rel_epsilon"];

    max_time_ = j["max_time"];

    gx_ = j["force_x"];
    gy_ = j["force_y"];

    safety_tau_ = j["safety_tau"];

    omega_ = j["omega"];

    SOR_max_it_ = j["SOR_max_it"];

    json_file.close();
}

index_t Options::getCellCntX() const
{
    return cell_cnt_x_;
}

index_t Options::getCellCntY() const
{
    return cell_cnt_y_;
}
    
double Options::getDx() const
{
    return dx_;
}

double Options::getDy() const
{
    return dy_;
}

double Options::getReynold() const
{
    return reynold_;
}

double Options::getRelEps() const
{
    return rel_epsilon_;
}

double Options::getMaxTime() const
{
    return max_time_;
}

double Options::getForceX() const
{
    return gx_;
}

double Options::getForceY() const
{
    return gy_;
}

double Options::getSafetyTau() const
{
    return safety_tau_;
}

double Options::getOmega() const
{
    return omega_;
}

index_t Options::getSORMaxIt() const
{
    return SOR_max_it_;
}