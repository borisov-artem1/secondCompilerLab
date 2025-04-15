cmake_minimum_required(VERSION 3.30)
project(secondCompLab)

set(CMAKE_CXX_STANDARD 20)

add_executable(secondCompLab src/main/main.cpp
        src/grammar/grammar_info.cpp
        src/grammar/grammar_info.h
        src/algorithms/left_recursion_deleter/left_recursion_deleter.cpp
        src/algorithms/left_recursion_deleter/left_recursion_deleter.h
        src/algorithms/chain_rules_deleter/chain_rules_deleter.cpp
        src/algorithms/chain_rules_deleter/chain_rules_deleter.h
)
