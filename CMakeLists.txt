cmake_minimum_required(VERSION 3.28)
project(lab4)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab4 main.cpp
        Test.cpp
        Graph.h
)

target_include_directories(lab4 PUBLIC
        Sequences
        PTRs
        Sorts
        dict
)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core Charts)

target_link_libraries(lab4 PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui Qt6::Charts)