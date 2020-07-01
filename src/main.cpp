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
        testStencils();

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

    matrix_t test_m = std::vector<std::vector<double>>(Op.getCellCntX(), std::vector<double>(1)); // testing out d/dx
    //matrix_t test_m = std::vector<std::vector<double>>(1, std::vector<double>(Op.getCellCntY())); //  testing out d/dy
    Stencils sten(Op.getDx(), Op.getDy(), 1);

    auto func = [](double x, double y) -> double { return sin(x); }; // change to sin(y) if testing out d/dy

    // fill with with function sin(x)
    for (index_t i = 0; i < test_m.size() ; i++) {
        for (index_t j = 0; j < test_m[i].size() ; j++) {
            test_m.at(i).at(j) = func(i * Op.getDx(), j * Op.getDy());
        }
    }

    auto test_deriv = std::vector<std::vector<double>>(test_m.size() - 2, std::vector<double>(1));  // testing out d/dx
                                                                                                    // two shorter to allow i-1 and i+1
    //auto test_deriv = std::vector<std::vector<double>>(1, std::vector<double>(test_m.at(0).size() - 2));    // testing out d/dy
                                                                                                            // two shorter to allow j-1 and j+1

    // calculate first derivative

    // testing out d/dx
    for (index_t i = 1; i <= test_deriv.size(); i++) { // iterate over x
        for (index_t j = 0; j < test_deriv.at(i-1).size(); j++) { // iterate over y
                test_deriv.at(i-1).at(j) = sten.secondDerivF(test_m, Stencils::PARAM::X, i, j);
        }
    }

    // testing out d/dy
    /*for (index_t i = 0; i < test_deriv.size(); i++) { // iterate over x
        for (index_t j = 1; j <= test_deriv.at(i).size(); j++) { // iterate over y
                test_deriv.at(i).at(j-1) = sten.secondDerivF(test_m, Stencils::PARAM::Y, i, j);
    }*/

    CSVPrinter printer;
    printer.printMatrix(test_deriv, "test_file");
}