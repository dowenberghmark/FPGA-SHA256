cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
project( cpu )
add_executable( app application_opt.cpp sha256.cpp )
