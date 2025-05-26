# ElbrusSPTasks

## Installation:

```sh
git submodule update --init --remote --recursive

cmake -DCURRENT_CONFIG=Release -S . -B build && $(cd build && make)
build/pthreads/parallelSort <array size> <threads count> [<seed> = 42]
```