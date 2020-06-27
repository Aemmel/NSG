//
// 
//

#include <iostream>
#include "State.hpp"
#include "CSVPrinter.hpp"

#include "Options.hpp"

int main()
{
    Options Op = Options("data/options.json");
    auto state = State(100, 100, 0.01, 0.01);
    CSVPrinter printer;
    printer.print(state, 0);
    //Output the state to files
    return 0;
}