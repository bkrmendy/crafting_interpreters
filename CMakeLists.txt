cmake_minimum_required(VERSION 3.20)
project(loxpp)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(include/fmt)

add_library(loxpp
            lib/Scanner.cpp
            lib/utils.cpp
            lib/AstPrinter.cpp
            lib/Value.cpp
            lib/Parser.cpp
            lib/AstInterpreter.cpp
            lib/TypeCast.cpp
            lib/RuntimeError.cpp
        )

include_directories(loxpp include)
target_link_libraries(loxpp fmt)

add_executable(loxc main.cpp)
include_directories(loxc include)
target_link_libraries(loxc loxpp fmt)

enable_testing()
add_subdirectory(test)