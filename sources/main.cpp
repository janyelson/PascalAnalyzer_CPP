#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <SyntaxAnalyzer.h>
#include "Tokenizer.h"

int main() {

    std::ifstream infile("Test.pascal");
    std::vector<std::string> lines;

    std::string line;


    while (std::getline(infile, line))
    {
        lines.push_back(line);
    }

    try
    {
        Tokenizer tokenizer(lines, true);
        tokenizer.parser();

        std::vector<Token> tokens(tokenizer.getTokens());

        for(Token &token : tokens)
        {
            std::cout << token.toString() << "\n\n";
        }

        SyntaxAnalyzer syntaxAnalyzer(tokens, true);
        syntaxAnalyzer.run();
    }
    catch (std::exception &ex)
    {
        ex.what();
    }

    return 0;
}