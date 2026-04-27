cmake_minimum_required(VERSION 3.16)
project(spreadsheet)

set(CMAKE_CXX_STANDARD 17)

add_executable(spreadsheet
    src/main.cpp
    src/SpreadSheet.cpp
)
