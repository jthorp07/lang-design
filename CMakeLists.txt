# Specify CMake minimum version
cmake_minimum_required(VERSION 3.31)

# Project init
project(LangDesign VERSION 0.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Options to manage what steps get built
option(STEP_ONE "Build step one" OFF)
if(STEP_ONE)
    message(STATUS "Adding step one build files.")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/step_one_tokenizer)
endif()

option(STEP_TWO "Build step two" OFF)
if(STEP_ONE)
    message(STATUS "Adding step two build files.")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/step_two_lexer)
endif()

# Warn if no steps are selected
if(NOT STEP_ONE AND NOT STEP_TWO)
    message(WARNING "No steps selected to build.")
endif()