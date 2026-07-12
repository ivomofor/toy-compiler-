#include <fstream>
#include <iostream>
#include <sstream>

#include "toy/Lexer.h"
#include "toy/Parser.h"
#include "toy/ASTPrinter.h"

int main(int argc, char **argv) {

    if (argc != 2) {
        std::cerr << "Usage: toy-parse <file>\n";
        return 1;
    }

    std::ifstream input(argv[1]);

    if (!input) {
        std::cerr << "Could not open file.\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << input.rdbuf();

    toy::Lexer lexer(buffer.str());

    toy::Parser parser(lexer);

    auto program = parser.parseProgram();

    toy::ASTPrinter printer;
    printer.print(*program);

    return 0;
}