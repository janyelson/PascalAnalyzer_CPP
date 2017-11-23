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

    SymbolsTable(const SymbolsTable &that)
    {
        stack = that.stack;
    }

    std::string & getType(const std::string &identifierName)
    {
        auto i = (stack.size() - 1);
        while(i >= 0) {
            if(stack.at(i).getName() == (identifierName)) return stack.at(i).getType();
            i--;
        }

        return vd;
    }

    void enterScope()
    {
        stack.emplace_back(MARK, MARK_TYPE);
    }

    void exitScope() {
        auto i = stack.size() - 1;
        while(stack.at(i).getName() != (MARK)) {
            stack.erase(stack.begin() + i);
            i--;
        }

        stack.erase(stack.begin() + i);
    }

    bool searchDuplicateDeclaration(const std::string &identifierName) {

        auto i = stack.size() - 1;
        while(stack.at(i).getName() != (MARK)) {
            if(stack.at(i).getName() == (identifierName)) return true;
            i--;
        }

        return false;
    }

    bool searchIdentifier(const std::string &identifierName) {
        auto i = stack.size() - 1;
        while(i >= 0) {
            if(stack.at(i).getName() == (identifierName)) return true;
            i--;
        }

        return false;
    }

    void addSymbol(const std::string &text, const std::string &type)
    {
        stack.emplace_back(text, type);
    }

    void assignType(std::string type) {
        auto i = stack.size() - 1;

        while(stack.at(i).getType() == "void") {
            stack.at(i).setType(type);
            i--;
        }
    }

    void assignParameters()
    {
        Symbol procedureSymbol = getLastProcedure();
        auto i = stack.size() - 1;

        while(stack.at(i).getType() != ("procedure")) {
            if(stack.at(i).getType() != (MARK_TYPE)) procedureSymbol.addParameter(stack.at(i));
            i--;
        }

    }

    Symbol & getLastProcedure()
    {

        auto i = stack.size() - 1;

        while(stack.at(i).getType() != ("procedure"))
        {
            i--;
        }
        return stack.at(i);
    }

    Symbol & getProcedure(std::string &procedureName) {
        auto i = stack.size() - 1;
        while(stack.at(i).getName() != (procedureName) && stack.at(i).getType() == ("procedure") )
        {
            i--;
        }

        return stack.at(i);
    }

    std::string getProgramName()
    {
        return stack.at(1).getName();
    }

};
#endif //PASCALANAYZER_SYMBOLSTABLE_H


