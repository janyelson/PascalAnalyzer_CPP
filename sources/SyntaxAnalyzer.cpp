#include <iostream>
#include "SyntaxAnalyzer.h"

bool containsType(const std::string &word)
{
    for(const std::string &type : Token::types)
    {
        if(word == type) return true;
    }

    return false;
}

void SyntaxAnalyzer::run() throw()
{
    std::cout << "Analyzing syntax..." << std::endl;
    count = 0;
    program();
    std::cout <<"Syntax is correct." << std::endl;
}

void SyntaxAnalyzer::program() throw()
{
    //keyword: 'program'
    currentToken = &tokens.at(count); //count == 0
    if (currentToken->getText() != "program")
    {
        syntaxError("Keyword 'program' was not found!");
    }

    //enter into global scope
    symbolsTable.enterScope();

    //identifier: name of the program
    getNextToken();
    if (currentToken->getClassification() != Token::Classifications::IDENTIFIER)
    {
        syntaxError("Invalid identifier for program!");
    }

    std::string pr = "program";
    Symbol symbol(currentToken->getText(), pr);
    symbolsTable.addSymbol(symbol);


    //identifier: ';'
    getNextToken();
    if (currentToken->getText() != ";")
    {
        syntaxError("Symbol ';' was not found!");
    }

    //control flow
    varDeclaration();
    subProgramsDeclaration();
    compoundCommand();

    //exit scope
    symbolsTable.exitScope();

    //delimiter: '.'
    getNextToken();
    if (currentToken->getText() != ".")
    {
        syntaxError("Symbol '.' was not found!");
    }
}

void SyntaxAnalyzer::varDeclaration() throw()
{
    getNextToken();
    if (currentToken->getText() == "var")
    {
        varDeclarationListA();
    }
    else
    {
        count--;
    }
}

void SyntaxAnalyzer::varDeclarationListA() throw()
{
    identifiersListA();

    getNextToken();
    if (currentToken->getText()!= ":")
    {
        syntaxError("Symbol ':' was not found!");
    }

    type();

    getNextToken();
    if (currentToken->getText() != ";")
    {
        syntaxError("Symbol ';' was not found!");
    }

    varDeclarationListB();
}

void SyntaxAnalyzer::varDeclarationListB() throw()
{
    getNextToken();
    if (currentToken->getClassification() != (Token::Classifications::IDENTIFIER))
    {
        count--;
        return;
    }
    count--;
    identifiersListA();

    getNextToken();
    if (currentToken->getText() != ":")
    {
        syntaxError("Symbol ':' was not found!");
    }

    type();

    getNextToken();
    if (currentToken->getText() != ";")
    {
        syntaxError("Symbol ';' was not found!");
    }

    varDeclarationListB();
}

void SyntaxAnalyzer::identifiersListA() throw()
{
    getNextToken();
    if (currentToken->getClassification() != (Token::Classifications::IDENTIFIER))
    {
        syntaxError("Invalid identifier!");
    }


    if(symbolsTable.getProgramName() == (currentToken->getText())) semanticError("Identifier has the same program name");
    //Checks whether the current identifier is declared elsewhere in the same scope
    if(symbolsTable.searchDuplicateDeclaration(currentToken->getText()))
    {
        semanticError("Duplicate identifier!");
    }


    //If not, put the identifier into stack
    Symbol symbol(currentToken->getText());
    symbolsTable.addSymbol(symbol);

    identifiersListB();
}

void SyntaxAnalyzer::identifiersListB() throw()
{
    getNextToken();
    if (currentToken->getText() != ",")
    {
        count--;
        return;
    }

    getNextToken();
    if (currentToken->getClassification() != (Token::Classifications::IDENTIFIER))
    {
        syntaxError("Invalid identifier!");
    }

    if(symbolsTable.getProgramName() == (currentToken->getText())) semanticError("Identifier has the same program name");
    //Checks whether the current identifier is declared elsewhere in the same scope
    if(symbolsTable.searchDuplicateDeclaration(currentToken->getText()))
    {
        semanticError("Duplicate identifier!");
    }


    //If not, put the identifier into stack
    Symbol symbol(currentToken->getText());
    symbolsTable.addSymbol(symbol);

    identifiersListB();
}

void SyntaxAnalyzer::type() throw()
{
    getNextToken();
    if (!containsType(currentToken->getText()))
    {
        syntaxError("Invalid type!");
    }

    symbolsTable.assignType(currentToken->getText());
}

void SyntaxAnalyzer::subProgramsDeclaration() throw()
{
    getNextToken();
    count--;
    if (currentToken->getText() == "procedure")
    {
        subProgram();
        subProgramsDeclaration();
    }
}

void SyntaxAnalyzer::subProgram() throw()
{
    getNextToken();
    if (currentToken->getText() != "procedure")
    {
        count--;
        syntaxError("Keyword 'procedure' was not found!");
    }

    getNextToken();
    if (currentToken->getClassification() != (Token::Classifications::IDENTIFIER))
    {
        count--;
        syntaxError("Invalid identifier!");
    }

    if(symbolsTable.getProgramName() == (currentToken->getText())) semanticError("Identifier has the same program name");
    //Checks whether the current identifier is declared elsewhere in the same scope
    if(symbolsTable.searchDuplicateDeclaration(currentToken->getText()))
    {
        semanticError("Duplicate identifier!");
    }


    //If not, put the identifier into stack
    std::string pr = "procedure";
    Symbol symbol(currentToken->getText(), pr);
    symbolsTable.addSymbol(symbol);

    //enter into local scope
    symbolsTable.enterScope();

    arguments();

    getNextToken();
    if (currentToken->getText() != ";")
    {
        count--;
        syntaxError("Symbol ';' was not found!");
    }

    varDeclaration();
    subProgramsDeclaration();
    compoundCommand();
    //exit scope
    symbolsTable.exitScope();

    getNextToken();
    if (currentToken->getText() != ";")
    {
        count--;
        syntaxError("Symbol ';' was not found!");
    }
}

void SyntaxAnalyzer::arguments() throw()
{
    getNextToken();
    if (currentToken->getText() != "(")
    {
        count--;
    }
    else
    {
        parameterListA();

        getNextToken();
        if (currentToken->getText() != ")")
        {
            syntaxError("Symbol ')' was not found!");
        }
    }
}

void SyntaxAnalyzer::parameterListA() throw()
{
    identifiersListA();
    getNextToken();
    if (currentToken->getText() != ":")
    {
       syntaxError("Symbol ':' was not found!");
    }

    type();

    parameterListB();
}

void SyntaxAnalyzer::parameterListB() throw()
{
    getNextToken();
    if (currentToken->getText() != ";")
    {
        count--;
        symbolsTable.assignParameters();
        return;
    }

    identifiersListA();

    getNextToken();
    if (currentToken->getText() != ":")
    {
        count--;
        syntaxError("Symbol ':' was not found!");
    }

    type();

    parameterListB();
}

void SyntaxAnalyzer::compoundCommand() throw()
{
    getNextToken();
    if (currentToken->getText() != "begin")
    {
        syntaxError("Keyword 'Begin' was not found!");
    }

    optionalCommands();

    getNextToken();
    if (currentToken->getText() != "end")
    {
        syntaxError("Keyword 'End' was not found!");
    }
}

void SyntaxAnalyzer::optionalCommands() throw()
{
    getNextToken();
    count--;
    if (currentToken->getText() != "end")
    {
        commandListA();
    }
}

void SyntaxAnalyzer::commandListA() throw()
{
    command();
    commandListB();
}

void SyntaxAnalyzer::commandListB() throw()
{
    getNextToken();
    if (currentToken->getText() == ";")
    {
        getNextToken();
        if (currentToken->getText() != "end")
        {
            count--;
            command();
            commandListB();
        }
        else
        {
            count--;
        }
    }
    else
    {
        count--;
    }
}

void SyntaxAnalyzer::command() throw()
{
    getNextToken();
    if (currentToken->getClassification() == (Token::Classifications::IDENTIFIER))
    {
        if(symbolsTable.getProgramName() == (currentToken->getText())) semanticError("Program name cannot be used");
        //Checks whether the current identifier is declared elsewhere
        if(!symbolsTable.searchIdentifier(currentToken->getText()))
        {
            semanticError("Using not declared identifier!");
        }


        std::string &resultClassification = symbolsTable.getType(currentToken->getText());
        getNextToken();
        if (currentToken->getText() == ":=")
        {
            typeControl.pushMark();
            expression();

            try{
                typeControl.popMark();
                typeControl.verifyResult(resultClassification);
            }catch (std::exception &e) {
                e.what();
            }

        }
        else
        {
            count -= 2;
            procedureActivationA();
            typeControl.reset();
        }
    }
    else if (currentToken->getText() == "if")
    {
        std::string resultClassification = "boolean";
        typeControl.pushMark();
        expression();

        try{
            typeControl.popMark();
            typeControl.verifyResult(resultClassification);
        }catch (std::exception &e) {
            e.what();
        }

        getNextToken();
        if (currentToken->getText() != "then")
        {
            count--;
            syntaxError("Keyword 'Then' was not found!");
        }

        commandStructure();

        elsePart();
    }
    else if (currentToken->getText() == "while")
    {
        std::string resultClassification = "boolean";
        typeControl.pushMark();
        expression();
        try{
            typeControl.popMark();
            typeControl.verifyResult(resultClassification);
        } catch (std::exception &e) {
            e.what();
        }

        getNextToken();
        if (currentToken->getText() != "do")
        {
            count--;
            syntaxError("Keyword 'do' was not found!");
        }

        commandStructure();
    }
    else if (currentToken->getText() == "do")
    {
        commandStructure();

        getNextToken();
        if (currentToken->getText() != "while")
        {
            count--;
            syntaxError("Keyword 'while' was not found!");
        }

        std::string resultClassification = "boolean";
        typeControl.pushMark();
        expression();
        try{
            typeControl.popMark();
            typeControl.verifyResult(resultClassification);
        } catch (std::exception &e) {
            e.what();
        }

    }
    else if (currentToken->getText() == "begin")
    {
        //compoundCommand also reads 'begin'
        count--;
        compoundCommand();
    }
    else
    {
        syntaxError("Invalid command.");
    }
}
void SyntaxAnalyzer::commandStructure() throw()
{
    getNextToken();
    count--;
    if (currentToken->getText() == "begin")
    {
        compoundCommand();
    }
    else
    {
        command();
    }
}

void SyntaxAnalyzer::elsePart() throw()
{
    getNextToken();
    if (currentToken->getText() == "else")
    {
        commandStructure();
    }
    else
    {
        count--;
    }
}

void SyntaxAnalyzer::procedureActivationA() throw()
{
    getNextToken();
    if (currentToken->getClassification() != Token::Classifications::IDENTIFIER)
    {
        syntaxError("Invalid identifier!");
    }

    typeControl.setCallProcedure(true, symbolsTable.getProcedure(currentToken->getText()));
    getNextToken();
    if (currentToken->getText() != "(")
    {
        count--;
        return;
    }

    count--;
    expressionListA();
}

void SyntaxAnalyzer::expressionListA() throw()
{
    getNextToken();
    if (currentToken->getText() != "(")
    {
        count--;
    }
    else
    {

        typeControl.pushMark();
        expression();


        try {
            typeControl.popMark();

            if(typeControl.isCallProcedure()) {
                typeControl.pushParameter(typeControl.getFirstType());
                typeControl.reset();
            }
        }  catch (std::exception &e) {
            e.what();
        }


        expressionListB();

        getNextToken();
        if (currentToken->getText() != ")")
        {
            syntaxError("Symbol ')' was not found!");
        }
    }
}

void SyntaxAnalyzer::expressionListB() throw()
{
    getNextToken();
    if (currentToken->getText()!= ",")
    {
        count--;


        try {
            typeControl.verifyResultProcedureCall();
        } catch (std::exception &e) {
            e.what();
        }



        return;
    }

    typeControl.pushMark();
    expression();


    try {
        typeControl.popMark();

        if(typeControl.isCallProcedure()) {
            typeControl.pushParameter(typeControl.getFirstType());
            typeControl.reset();
        }
    } catch (std::exception &e) {
        e.what();
    }


    expressionListB();
}

void SyntaxAnalyzer::expression() throw()
{
    simpleExpressionA();

    getNextToken();
    if (currentToken->getClassification() != (Token::Classifications::RELATIONAL))
    {
        count--;
    }
    else
    {
        std::string rel = "relational";
        typeControl.pushOperation(rel);
        simpleExpressionA();
    }
}


void SyntaxAnalyzer::simpleExpressionA() throw()
{
    getNextToken();
    if (currentToken->getText() != "+" && currentToken->getText() != "-")
    {
        count--;
    }

    termA();
    simpleExpressionB();
}

void SyntaxAnalyzer::simpleExpressionB() throw()
{
    getNextToken();
    //stop simpleExpressionB's loop
    if (currentToken->getClassification() != (Token::Classifications::ADDITION))
    {
        count--;
    }
    else
    {
        std::string add = "addition";
        typeControl.pushOperation(add);
        termA();
        simpleExpressionB();
    }
}

void SyntaxAnalyzer::termA() throw()
{
    factor();
    termB();
}

void SyntaxAnalyzer::termB() throw()
{
    getNextToken();
    if (currentToken->getClassification() == (Token::Classifications::MULTIPLICATION))
    {
        std::string mult = "multiplication";
        typeControl.pushOperation(mult);
        factor();
        termB();
    }
    else
    {
        count--;
    }
}
void SyntaxAnalyzer::factor() throw()
{
    getNextToken();
    if (currentToken->getClassification() == (Token::Classifications::IDENTIFIER))
    {

        if(symbolsTable.getProgramName() == (currentToken->getText())) semanticError("Program name cannot be used");
        //Checks whether the current identifier is declared elsewhere
        if(!symbolsTable.searchIdentifier(currentToken->getText()))
        {
            semanticError("Using not declared identifier!");
        }


        try {
            std::string &type = symbolsTable.getType(currentToken->getText());
            typeControl.pushType(type);
        } catch (std::exception &e) {
            e.what();

        }
        getNextToken();
        count--;
        if (currentToken->getText() == "(")
        {
            expressionListA();
        }
    }
    else if (currentToken->getText() == "(")
    {
        typeControl.pushMark();
        expression();


        try {
            typeControl.popMark();
        } catch (std::exception &e) {
            e.what();
        }


        getNextToken();
        if (currentToken->getText() != ")")
        {
            count--;
            syntaxError("Symbol ')' was not found!");
        }
    }
    else if (currentToken->getText() == "not")
    {
        factor();
    }
    else if (containsType(currentToken->getClassificationName()))
    {
        try{
            std::string type = currentToken->getClassificationName();
            typeControl.pushType(type);
        } catch (std::exception &e) {
            e.what();
        }
    }
    else {
        count--;
        syntaxError("Expected factor.");
    }
}

