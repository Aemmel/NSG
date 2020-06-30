//
// Created by janhb on 26.06.2020.
//

#include "CSVPrinter.hpp"

#include <utility>


void CSVPrinter::print(State state, double time) {
    std::ofstream file;

    //Print u
    file.open(file_prefix_ + "u_" + std::to_string(time) + "." + extension_);
    CSVPrinter::print_vector(file, state.u);
    file.close();

    //Print v
    file.open(file_prefix_ + "v_" + std::to_string(time) + "." + extension_);
    CSVPrinter::print_vector(file, state.v);
    file.close();

    //Print p
    file.open(file_prefix_ + "p_" + std::to_string(time) + "." + extension_);
    CSVPrinter::print_vector(file, state.p);
    file.close();
}

void CSVPrinter::print_vector(std::ofstream& stream, std::vector<std::vector<double>>& array)
{
    // prevent trailing tab
    // might not be elegant, but it works
    uint_fast32_t row_cnt = 0;
    char delim = '\t';

    //Iterate over each row
    for (const std::vector<double>& row : array) {
        //Iterate over each column
        for (double element : row) {
            delim = (row_cnt < row.size() - 1) ? '\t' : ' '; // use tab unless we're in the last column, then use harmless space

            stream << element << delim;

            row_cnt++;
        }
        stream << std::endl;
        row_cnt = 0;
    }
}

CSVPrinter::CSVPrinter(std::string file_prefix, std::string extension) {
    file_prefix_ = std::move(file_prefix);
    extension_ = std::move(extension);
}
