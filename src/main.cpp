//
// 
//

#include <iostream>
#include "State.hpp"
#include "CSVPrinter.hpp"

int main()
{
    auto state = State(100, 100, 0.01, 0.01);

    CSVPrinter printer;

    //Output the state to files
    printer.print(state, 0);

    return 0;
}