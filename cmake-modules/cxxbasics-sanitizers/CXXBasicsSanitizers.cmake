cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

include(sanitizers/UseASan)
