#include <fmt/core.h>
#include <iostream>
#include <fstream>
#include <sstream>

void run(const std::string& source) {
    fmt::print("TODO: implement run");
}

void run_file(const std::string& path) {
    std::ifstream file{path};
    std::stringstream buffer{};
    buffer << file.rdbuf();
    run(buffer.str());
}

void run_prompt() {
    auto running = true;
    while (running) {
        std::cout << "In  : ";
        std::string buffer{};
        std::getline(std::cin, buffer);

        fmt::print("Out : {}\n", buffer);

        running = buffer != "exit";
    }
    fmt::print("I'm done with this");
}

int main(int argc, char** argv) {
    if (argc > 2) {
        fmt::print("Usage: lox++ [file]");
        std::exit(64);
    }

    if (argc == 2) {
        run_file(argv[1]);
    } else {
        run_prompt();
    }

    return 0;
}
