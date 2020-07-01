# OpenMP benchmark experiments

## Fill State object with functions
`#pragma omp parallel for collapse(2)`

Time measured by `time bin/NSG`. Time given is the "Real value", if not explicitly marked.

### State size: 10000 x 10000:

CPU-time With OpenMP activated (and set OMP_NUM_THREADS=8): 49.092s

Real-time with OpenMP activated: 8.902s


Without OpenMP: 39.032s


