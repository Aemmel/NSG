# NSG

## Build
Run `cmake .` to generate a Makefile, run `make` to create an executable.

## Execute
It is important that you run the compiled program from main folder (otherwise the wrong files will be opened).
Therefore: `./bin/NSG`

## Conventions
To make indexing consistent with the instructions, we ALWAYS index `[x][y]`
This means we first transpose the matrix, so the rows indicate our x and the columns indicate our y
This is due to how the computer saves a 2D array as opposed to the convention first `x`, then `y`