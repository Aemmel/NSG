//
// Created by janhb on 26.06.2020.
//

#ifndef NSG_CSVPRINTER_HPP
#define NSG_CSVPRINTER_HPP


#include "AbstractPrinter.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class CSVPrinter: public AbstractPrinter {
private:
    std::string file_prefix_;
    std::string extension_;

    static void printVector(std::ofstream& stream, const matrix_t& array);

public:
    CSVPrinter(): CSVPrinter("out/nsg_", "dat") {};
    CSVPrinter(std::string file_prefix, std::string extension);
    void print(const State &state, double time) override;

    void printMatrix(const matrix_t& m, std::string file_name);
};


#endif //NSG_CSVPRINTER_HPP
