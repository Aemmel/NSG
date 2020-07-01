//
// 
//

#include <iostream>
#include <cmath>

#include "State.hpp"
#include "CSVPrinter.hpp"
#include "Options.hpp"
#include "Stencils.hpp"

void testStencils();
void testStencilFG();

int main()
{
    try {
        Options Op = Options("data/options.json");
        auto state = State(Op.getCellCntX(), Op.getCellCntY(), Op.getDx(), Op.getDy());

        //Fill with known functions
        /*state.fillWithFunction(State::GRID::U, [](double y, double x) { return cos(x) * cos(y); });
        state.fillWithFunction(State::GRID::V, [](double y, double x) { return sin(x) * sin(y); });

        state.fillWithFunction(State::GRID::P, [](double y, double x) { return sin(x) * cos(y); });*/

        // try out stencils with known functions
        //testStencils();
        testStencilFG();

        CSVPrinter printer;
        //printer.print(state, 0);
        //Output the state to files
    }
    catch (const std::exception& e) {
        std::cerr << "UwU. Something went tewwibwy wwong: " << e.what() << std::endl;
    }
    return 0;
}

void testStencils()
{
    Options Op = Options("data/options.json");

    bool test_for_dx = true; // true: d/dx; false: d/dy

    matrix_t test_m;
    if (test_for_dx == true)
        matrix_t test_m = std::vector<std::vector<double>>(Op.getCellCntX(), std::vector<double>(1)); // testing out d/dx
    else
        matrix_t test_m = std::vector<std::vector<double>>(1, std::vector<double>(Op.getCellCntY())); //  testing out d/dy
    
    Stencils sten(Op.getDx(), Op.getDy(), 0.1);

    auto func = [test_for_dx](double x, double y) -> double { 
        if (test_for_dx == true) 
            return sin(x); 
        else
            return sin(y);
    };

    // fill with with function sin(x)
    for (index_t i = 0; i < test_m.size() ; i++) {
        for (index_t j = 0; j < test_m[i].size() ; j++) {
            test_m.at(i).at(j) = func(i * Op.getDx(), j * Op.getDy());
        }
    }

    matrix_t test_deriv;

    if (test_for_dx == true) {
        test_deriv = std::vector<std::vector<double>>(test_m.size() - 2, std::vector<double>(1));   // testing out d/dx
                                                                                                    // two shorter to allow i-1 and i+1
    }
    else {
        test_deriv = std::vector<std::vector<double>>(1, std::vector<double>(test_m.at(0).size() - 2)); // testing out d/dy
                                                                                                        // two shorter to allow j-1 and j+1
    }
    
    if (test_for_dx == true) {
        for (index_t i = 1; i <= test_deriv.size(); i++) { // iterate over x
            for (index_t j = 0; j < test_deriv.at(i-1).size(); j++) { // iterate over y
                    test_deriv.at(i-1).at(j) = sten.firstDerivFSquared(test_m, Stencils::PARAM::X, i, j);
            }
        }
    }
    else {
        for (index_t i = 0; i < test_deriv.size(); i++) { // iterate over x
            for (index_t j = 1; j <= test_deriv.at(i).size(); j++) { // iterate over y
                test_deriv.at(i).at(j-1) = sten.firstDerivFSquared(test_m, Stencils::PARAM::Y, i, j);
            }
        }
    }

    CSVPrinter printer;
    printer.printMatrix(test_deriv, "test_file");
}

void testStencilFG()
{
    Options Op = Options("data/options.json");

    bool test_for_dx = false; // true: d/dx; false: d/dy

    Stencils sten(Op.getDx(), Op.getDy(), 0.1);

    matrix_t test_f;
    matrix_t test_g;
    matrix_t test_deriv;
    if (test_for_dx == true) {
        test_f = std::vector<std::vector<double>>(Op.getCellCntX(), std::vector<double>(3)); // testing out d/dx
        test_deriv =  std::vector<std::vector<double>>(Op.getCellCntX() - 2, std::vector<double>(1));
    }
    else {
        test_f = std::vector<std::vector<double>>(3, std::vector<double>(Op.getCellCntY())); //  testing out d/dy
        test_deriv = std::vector<std::vector<double>>(1, std::vector<double>(Op.getCellCntY() - 2));
    }
    test_g = test_f;

    auto func_f = [test_for_dx](double x, double y) -> double { 
        if (test_for_dx == true) 
            return sin(x); 
        else
            return sin(y);
    };

    auto func_g = [test_for_dx](double x, double y) -> double { 
        if (test_for_dx == true) 
            return cos(x); 
        else
            return cos(y);
    };

    // fill with with function sin(x)*cos(x)
    for (index_t i = 0; i < test_f.size() ; i++) {
        for (index_t j = 0; j < test_f[i].size() ; j++) {
            test_f.at(i).at(j) = func_f(i * Op.getDx(), j * Op.getDy());
            test_g.at(i).at(j) = func_g(i * Op.getDx(), j * Op.getDy());
        }
    }

    if (test_for_dx == true) {
        for (index_t i = 1; i <= test_deriv.size(); i++) { // iterate over x
            test_deriv.at(i-1).at(0) = sten.firstDerivFG(test_f, test_g, Stencils::PARAM::X, i, 1);
        }
    }
    else {
        for (index_t j = 1; j <= test_deriv[0].size(); j++) { // iterate over y
            test_deriv.at(0).at(j-1) = sten.firstDerivFG(test_f, test_g, Stencils::PARAM::Y, 1, j);
        }
    }

    CSVPrinter printer;
    printer.printMatrix(test_deriv, "test_file");
}