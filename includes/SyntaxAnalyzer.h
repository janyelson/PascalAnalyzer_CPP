#ifndef PASCALANAYZER_SYNTAXANALYZER_H
#define PASCALANAYZER_SYNTAXANALYZER_H

#include "Token.h"
#include "SyntaxException.h"
#include "SymbolsTable.h"
#include "TypeControl.h"
#include <vector>
#include <string>

class SyntaxAnalyzer
{
private:
    /*PROPERTIES*/
    std::vector<Token> tokens;
    Token* currentToken = nullptr;
    unsigned int count{};
    bool DEBUG_MODE = false;

    /*SEMANTIC ANALYZER*/
    SymbolsTable symbolsTable;
    TypeControl typeControl;

public:
    SyntaxAnalyzer(std::vector<Token> &t, bool D) : tokens(t), DEBUG_MODE(D) {}

    SyntaxAnalyzer (const SyntaxAnalyzer &that)
    {
        tokens = that.tokens;
        currentToken = that.currentToken;
        count = that.count;
        DEBUG_MODE = that.DEBUG_MODE;

        symbolsTable = that.symbolsTable;
        typeControl = that.typeControl;
    }

    explicit SyntaxAnalyzer(std::vector<Token> &tokens) : SyntaxAnalyzer(tokens, true) {}

    SyntaxAnalyzer() = default;

    void run() throw();

private:
    void program()throw();
    void varDeclaration()throw();
    void varDeclarationListA()throw();
    void varDeclarationListB()throw();
    void identifiersListA()throw();
    void identifiersListB()throw();
    void type()throw();
    void subProgramsDeclaration()throw();
    void subProgram()throw();
    void arguments()throw();
    void parameterListA()throw();
    void parameterListB()throw();
    void compoundCommand()throw();
    void optionalCommands()throw();
    void commandListA()throw();
    void commandListB()throw();
    void command()throw();
    void commandStructure()throw();
    void elsePart()throw();
    void procedureActivationA()throw();
    void expressionListA()throw();
    void expressionListB()throw();
    void expression()throw();
    void simpleExpressionA()throw();
    void simpleExpressionB()throw();
    void termA()throw();
    void termB()throw();
    void factor() throw();

    void getNextToken()
    {
        count++;
        if (count >= tokens.size())
        {
            count = static_cast<unsigned int>(tokens.size() - 1);
            syntaxError("No more tokens to be read.");
        }
        if (this->DEBUG_MODE)
        {
            std::cout << "Reading token: " << tokens.at(count).getText() << std::endl;
        }
        currentToken = &tokens.at(count);
    }

    bool containsType(const std::string &word) const
    {
        for(const std::string &type : Token::types)
        {
            if(word == type) return true;
        }

        return false;
    }

    void syntaxError(const std::string &errorMsg) throw()
    {
        std::string str = "Syntax Error! Line " + std::to_string(currentToken->getLineNumber()) + ":\n"
                          + currentToken->getText() + "\n" + errorMsg;
        throw SyntaxException(str);
    }

    void semanticError(const std::string &errorMsg) throw()
    {
        std::string str = "Semantic Error! Line " + std::to_string(currentToken->getLineNumber()) + ":\n" +
                currentToken->getText() + "\n" + errorMsg;
        throw SemanticException(str);
    }
};
#endif //PASCALANAYZER_SYNTAXANALYZER_H
