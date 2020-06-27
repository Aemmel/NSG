#include "json.hpp"
#include <iostream>
#include <fstream>

#include "Options.hpp"

using json = nlohmann::json;
using std::cout;
using std::endl;

Options::Options(std::string file_name)
{
    std::ifstream json_file(file_name);
    json j;
    json_file >> j;

    cell_cnt_x_ = j["cell_cnt_x"];
    cell_cnt_y_ = j["cell_cnt_y"];

    dx_ = j["dx"];
    dy_ = j["dy"];

    reynold_ = j["reynold"];

    rel_epsilon_ = j["rel_epsilon"];

    max_time_ = j["max_time"];

    cout << reynold_ << endl;

    json_file.close();
}

uint_fast32_t Options::getCellCntX()
{
    return cell_cnt_x_;
}

uint_fast32_t Options::getCellCntY()
{
    return cell_cnt_y_;
}
    
double Options::getDx()
{
    return dx_;
}

double Options::getDy()
{
    return dy_;
}

double Options::getReynold()
{
    return reynold_;
}

double Options::getRelEps()
{
    return rel_epsilon_;
}

double Options::getMaxTime()
{
    return max_time_;
}