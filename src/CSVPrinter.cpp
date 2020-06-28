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
    //Iterate over each row
    for (const std::vector<double>& row : array) {
        //Iterate over each column
        for (double element : row) {
            stream << element << "\t";
        }
        stream << std::endl;
    }
}

CSVPrinter::CSVPrinter(std::string file_prefix, std::string extension) {
    file_prefix_ = std::move(file_prefix);
    extension_ = std::move(extension);
}
