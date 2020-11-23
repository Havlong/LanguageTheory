cmake_minimum_required(VERSION 3.17)
project(LanguageTheory)

set(CMAKE_CXX_STANDARD 17)

add_executable(LanguageTheory main.cpp Parser.cpp Parser.h Interpreter.cpp Interpreter.h SyntaxTreeNode.h Static.h)
