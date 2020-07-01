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

    dx_ = j["dx"];
    dy_ = j["dy"];

    reynold_ = j["reynold"];

    rel_epsilon_ = j["rel_epsilon"];

    max_time_ = j["max_time"];

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