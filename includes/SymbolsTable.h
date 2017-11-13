//
// Created by MEU PC on 12/11/2017.
//

#ifndef PASCALANAYZER_SYMBOLSTABLE_H
#define PASCALANAYZER_SYMBOLSTABLE_H

#include <string>
#include <vector>
#include "Symbol.h"
#include "ProcedureSymbol.h"

class SymbolsTable
{
    std::string MARK = "$";
    std::string MARK_TYPE = "markType";
    std::vector<Symbol> stack;
    std::string vd = "void";

public:

    SymbolsTable() = default;

    std::string & getType(std::string &identifierName) {
        int i = stack.size() - 1;
        while(i >= 0) {
            if(stack.at(i).getName() == (identifierName)) return stack.at(i).getType();
            i--;
        }

        return vd;
    }

    void enterScope()
    {
        stack.push_back(Symbol(MARK, MARK_TYPE));
    }

    void exitScope() {
        int i = stack.size() - 1;
        while(stack.at(i).getName() != (MARK)) {
            stack.erase(stack.begin() + i);
            i--;
        }

        stack.erase(stack.begin() + i);
    }

    bool searchDuplicateDeclaration(std::string &identifierName) {

        int i = stack.size() - 1;
        while(stack.at(i).getName() != (MARK)) {
            if(stack.at(i).getName() == (identifierName)) return true;
            i--;
        }

        return false;
    }

    bool searchIdentifier(std::string &identifierName) {
        int i = stack.size() - 1;
        while(i >= 0) {
            if(stack.at(i).getName() == (identifierName)) return true;
            i--;
        }

        return false;
    }

    void addSymbol(Symbol symbol) {
        stack.push_back(symbol);
    }

    void assignType(std::string type) {
        int i = stack.size() - 1;

        while(stack.at(i).getType() == "void") {
            stack.at(i).setType(type);
            i--;
        }
    }

    void assignParameters()
    {
        Symbol procedureSymbol = getLastProcedure();
        int i = stack.size() - 1;

        while(stack.at(i).getType() != ("procedure")) {
            if(stack.at(i).getType() != (MARK_TYPE)) procedureSymbol.addParameter(stack.at(i));
            i--;
        }

    }

    Symbol & getLastProcedure() {

        int i = stack.size() - 1;

        while(stack.at(i).getType() != ("procedure")) {
            i--;
        }
        return stack.at(i);
    }

    Symbol & getProcedure(std::string &procedureName) {
        int i = stack.size() - 1;
        while(stack.at(i).getName() != (procedureName) && stack.at(i).getType() == ("procedure") ) {
            i--;
        }

        return stack.at(i);
    }

    std::string getProgramName() {
        return stack.at(1).getName();
    }

};
#endif //PASCALANAYZER_SYMBOLSTABLE_H


