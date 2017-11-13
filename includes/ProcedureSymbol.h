//
// Created by MEU PC on 12/11/2017.
//

#ifndef PASCALANAYZER_PROCEDURESYMBOL_H
#define PASCALANAYZER_PROCEDURESYMBOL_H

#include "Symbol.h"
#include <string>
#include <vector>

class ProcedureSymbol : Symbol {

    std::vector<Symbol> parameters;

public:
    explicit ProcedureSymbol(std::string &name) : Symbol(name, (std::string &) "procedure") {}

    ProcedureSymbol() = default;

/**
 * Take the amount of parameters.
 *
 * @return return amount of parameters.
 */
    int getParametersSize() {
        return parameters.size();
    }

/**
 * Get parameter name located at i position.
 *
 * @param i position
 * @return return a name parameter.
 */
    Symbol getParamater(int i) {
        return parameters.at(i);
    }

/**
 * Add a parameter to this procedure.
 *
 * @param parameter Symbol
 */
    void addParameter(Symbol &parameter)
    {
        parameters.push_back(parameter);
    }

};
#endif //PASCALANAYZER_PROCEDURESYMBOL_H
