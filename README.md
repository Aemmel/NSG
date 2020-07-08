# NSG

## Build
Run `cmake .` to generate a Makefile, run `make` to create an executable.

Use `cmake -DCMAKE_BUILD_TYPE=Release .` to compile with optimizations.

## Execute
It is important that you run the compiled program from main folder (otherwise the wrong files will be opened).
Therefore: `./bin/NSG`

## Conventions
### Indexing
To make indexing consistent with the instructions, we ALWAYS index `[x][y]`
This means we first transpose the matrix, so the rows indicate our x and the columns indicate our y
This is due to how the computer saves a 2D array as opposed to the convention first `x`, then `y`

### Grid size
The true grid on which we are calculating goes from `i = 1,..., width` and from `j = 1,..., height`.
However we need ghost cells, so we add `i = 0, i = width+1, j = 0, j = height+1`.
To make it easier to work with, we save the entire grid with the ghost cells. Meaning `cell_cnt_x`and `cell_cnt_y` already include our ghost cells. So if we want to iterate over the grid (w/out the ghost cells), we start from `i,j = 1` and end at `i,j = cell_cnt_x-1, cell_cnt_y-1`.
