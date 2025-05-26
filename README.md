# ElbrusSPTasks

## Installation:

```sh
git submodule update --init --remote --recursive

cmake -DCURRENT_CONFIG=Release -S . -B build && $(cd build && make)
python3 main.py
build/matrixSigint/sigint
```