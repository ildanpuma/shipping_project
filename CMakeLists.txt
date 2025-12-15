cmake_minimum_required(VERSION 4.0)
project(shipping_project C)

set(CMAKE_C_STANDARD 11)

add_executable(shipping_project main.c
        data_types.h
        sort_util.c
        data_struct.c
        sort_util.h
        util.h
        util.c)
