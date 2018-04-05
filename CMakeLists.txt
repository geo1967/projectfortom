cmake_minimum_required(VERSION 3.10)
project(project)

set(CMAKE_CXX_STANDARD 11)

add_executable(project
        main.cpp Experiment.h Measurement.h Linear.cpp Measurement.cpp Constant.cpp)
