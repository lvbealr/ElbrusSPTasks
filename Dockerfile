FROM ubuntu:18.04

RUN apt-get update && apt-get install -y    \
    build-essential                         \
    cmake                                   \
    g++                                     \
    python3-dev                             \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN mkdir build && cd build &&              \
    cmake -DCURRENT_CONFIG=Debug .. &&      \
    make