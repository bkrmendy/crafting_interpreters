cmake_minimum_required(VERSION 3.20)
project(loxpp)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(include/fmt)

add_executable(loxpp main.cpp include/TokenType.h include/Token.h include/Scanner.h src/Scanner.cpp include/utils.h src/utils.cpp include/LoxValue.h)

include_directories(loxpp include)
target_link_libraries(loxpp fmt)
