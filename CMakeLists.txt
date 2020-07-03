cmake_minimum_required(VERSION 3.9)
project(NSG CXX)

set(CMAKE_CXX_STANDARD 11)


add_compile_options(-Wall)

# Output directory for binaries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

add_executable(NSG
        src/main.cpp
        src/State.cpp
        src/State.hpp
        src/AbstractPrinter.hpp
        src/CSVPrinter.cpp
        src/CSVPrinter.hpp
        src/Options.hpp
        src/Options.cpp
        src/Stencils.hpp
        src/Stencils.cpp
        src/json.hpp
        src/TimeStepper.cpp
        src/TimeStepper.hpp
        src/AbstractBoundaryCondidtion.hpp
        src/SOR.hpp
        src/SOR.cpp
        src/CavityFlowBoundaryCondition.cpp
        src/CavityFlowBoundaryCondition.hpp)

# Try to use OpenMP to use multithreading
find_package(OpenMP)

# Try to use OpenMP to use multithreading
if(OpenMP_CXX_FOUND)
    #Uncomment to enable OpenMP
    target_link_libraries(NSG PUBLIC OpenMP::OpenMP_CXX)
endif()