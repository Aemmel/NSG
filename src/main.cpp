//
// 
//

#include <iostream>
#include <cmath>

#include "State.hpp"
#include "CSVPrinter.hpp"

#include "Options.hpp"

int main()
{
    //Options Op = Options("data/options.json");
    auto state = State(100, 100, 0.1, 0.1);


    //Fill with known functions
    state.fillWithFunction(State::GRID::U, [](double x, double y) { return cos(x) * cos(y); });
    state.fillWithFunction(State::GRID::V, [](double x, double y) { return sin(x) * sin(y); });

    state.fillWithFunction(State::GRID::P, [](double x, double y) { return sin(x) * cos(y); });

    CSVPrinter printer;
    printer.print(state, 0);
    //Output the state to files
    return 0;
}