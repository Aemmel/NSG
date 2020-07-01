//
// Created by janhb on 26.06.2020.
//

#include "CSVPrinter.hpp"
#include "Common.hpp"

#include <utility>

CSVPrinter::CSVPrinter(std::string file_prefix, std::string extension) {
    file_prefix_ = std::move(file_prefix);
    extension_ = std::move(extension);
}

void CSVPrinter::printVector(std::ofstream& stream, const matrix_t& array)
{
    if (stream.is_open() == false) {
        throw std::runtime_error("Failed to open file! und Jan stinkt");
    }

    // prevent trailing tab
    char delim = '\t';

    /*//Iterate over each row
    for (const std::vector<double>& row : array) {
        //Iterate over each column
        for (double element : row) {
            delim = (row_cnt < row.size() - 1) ? '\t' : ' '; // use tab unless we're in the last column, then use harmless space

            stream << element << delim;
        }
        stream << std::endl;
    }*/


    // remember that our array is transposed
    // so we have to first go over the columns and then over the rows
    // since we always have rectangular grids this should be fine. Otherwise this would break, since not every row is array[0].size() long
    for (index_t j = 0; j < array[0].size(); j++) {
        for (index_t i = 0; i < array.size(); i++) {
            delim = (i < array.size() - 1) ? '\t' : ' '; // use tab unless we're in the last column, then use harmless space

            stream << array[i][j] << delim;
        }
        stream << std::endl;
    }
}

void CSVPrinter::print(State state, double time) {
    std::ofstream file;

    //Print u
    file.open(file_prefix_ + "u_" + std::to_string(time) + "." + extension_);
    CSVPrinter::printVector(file, state.u);
    file.close();

    //Print v
    file.open(file_prefix_ + "v_" + std::to_string(time) + "." + extension_);
    CSVPrinter::printVector(file, state.v);
    file.close();

    //Print p
    file.open(file_prefix_ + "p_" + std::to_string(time) + "." + extension_);
    CSVPrinter::printVector(file, state.p);
    file.close();
}

void CSVPrinter::printMatrix(const matrix_t& m, std::string file_name)
{
    std::ofstream file;
    file.open(file_prefix_ + file_name + "." + extension_);

    printVector(file, m);

    file.close();
}