cmake_minimum_required(VERSION 3.19)
project(compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(compiler main.cpp Token.cpp Token.h allheader.h getSym.cpp getSym.h parsing.cpp parsing.h TableRow.cpp TableRow.h Quaternion.cpp Quaternion.h GenPcode.cpp GenPcode.h PcodeInstr.cpp PcodeInstr.h Interpreter.cpp Interpreter.h)