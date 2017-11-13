#ifndef PASCALANAYZER_SYMBOL_H
#define PASCALANAYZER_SYMBOL_H

#include <string>
#include <vector>
#include <iostream>

class Symbol
{
    std::string name;
    std::string type;
    std::vector<Symbol>* parameters = nullptr;

public:
    Symbol(std::string &n, std::string &t) : name(n), type(t)
    {
        if(type == "procedure")
        {
            parameters = new std::vector<Symbol>();
        }
    }

    explicit Symbol(std::string &n) : name(n), type("void"){}

    Symbol() = default;

    /**
     * Get a name of identifier.
     *
     * @return name of identifier
     */
    std::string getName() {
        return name;
    }

    /**
     * Get a type of identifier.
     *
     * @return type of identifier
     */
    std::string & getType() {
        return type;
    }

    /**
     * Add a type to the identifier.
     *
     * @param type will be add to identifier
     */
    void setType(std::string &type){
        this->type = type;
    }

    /**
 * Take the amount of parameters.
 *
 * @return return amount of parameters.
 */
    int getParametersSize() {
        return parameters->size();
    }

/**
 * Get parameter name located at i position.
 *
 * @param i position
 * @return return a name parameter.
 */
    Symbol getParamater(int i) {
        return parameters->at(i);
    }

/**
 * Add a parameter to this procedure.
 *
 * @param parameter Symbol
 */
    void addParameter(Symbol &parameter)
    {
        parameters->push_back(parameter);
    }
};
#endif //PASCALANAYZER_SYMBOL_H
