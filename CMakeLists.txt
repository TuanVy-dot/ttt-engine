cmake_minimum_required(VERSION 3.10)

project(tic-ai)

include_directories(include)

add_library(engine STATIC src/engine/board.c src/engine/eval.c)
