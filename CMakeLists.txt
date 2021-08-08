cmake_minimum_required(VERSION 3.20)
project(loxpp)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(include/fmt)

add_executable(loxpp
        main.cpp
        include/TokenType.h
        include/Token.h
        include/Scanner.h
        include/utils.h
        include/Value.h
        include/Expr.h

        src/Scanner.cpp
        src/utils.cpp
        include/Visitor.h include/AstPrinter.h src/AstPrinter.cpp src/Value.cpp include/Parser.h src/Parser.cpp include/ParseError.h)

include_directories(loxpp include)
target_link_libraries(loxpp fmt)
