# ElbrusSPTasks

## Installation:

```sh
git submodule update --init --remote --recursive

docker build -t process-syscall .
docker run -it process-syscall

cd syscall && python3 main.py <files count> && cd ../
# copy files names
build/syscall/processSyscall <files count> <files names>
```