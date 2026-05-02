cmake_minimum_required(VERSION 3.16)

project(spreadsheet LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(spreadsheet
        src/main.cpp
        src/mainWindow.cpp
        src/mainWindow.h
        src/spreadSheet.cpp
        src/spreadSheet.h
        src/cell.h
)

target_link_libraries(spreadsheet PRIVATE Qt6::Widgets)

if (WIN32)
    set_target_properties(spreadsheet PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
