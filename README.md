# ElbrusSPTasks

## Installation:

```sh
git submodule update --init --remote --recursive

cmake -DCURRENT_CONFIG=Release -S . -B build && $(cd build && make)
build/sortStrings/sortStrings sortStrings/texts/oneginEng.txt <flag (-f | -b | -l | -w | -v)>
```