cmake_minimum_required(VERSION 3.20)
project(loxpp)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(include/fmt)

add_library(loxpp SHARED
        lib/AstInterpreter.cpp
        lib/AstPrinter.cpp
        lib/Parser.cpp
        lib/RuntimeError.cpp
        lib/Scanner.cpp
        lib/TypeCast.cpp
        lib/utils.cpp
        lib/Value.cpp
        lib/Expr.cpp
        lib/Expression.cpp
        lib/Environment.cpp
        )

include_directories(loxpp include)
target_link_libraries(loxpp fmt)

add_executable(loxc main.cpp)
include_directories(loxc include)
target_link_libraries(loxc loxpp fmt)

enable_testing()
add_subdirectory(test)