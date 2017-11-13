//
// Created by MEU PC on 13/11/2017.
//

#ifndef PASCALANAYZER_TYPECONTROL_H
#define PASCALANAYZER_TYPECONTROL_H

#include <string>
#include <vector>
#include "Symbol.h"
#include "ProcedureSymbol.h"
#include "SemanticException.h"

class TypeControl
{
    std::string MARK = "$";
    std::vector<std::string> stackExpression;
    std::vector<std::string> operationStack;
    std::vector<std::string> procedureParametersStack;

    bool callProcedure = false;
    Symbol* procedureSymbol = nullptr;

public:

    TypeControl() = default;

    void pushMark() {
        stackExpression.push_back(MARK);
    }

    void popMark() throw()
    {
            int i = stackExpression.size()-1;
            std::string &result = stackExpression.at(i);

            stackExpression.erase(stackExpression.begin() + i);
            i--;
            stackExpression.erase(stackExpression.begin() + i);

            pushType(result);

    }

    void pushType(std::string &type) throw()
    {
            stackExpression.push_back(type);

            int i = stackExpression.size()-1;
            if(i < 1) return;

            int count = 0;
            while (stackExpression.at(i) != (MARK)) {
                count++;
                i--;
            }


            int x1 = stackExpression.size()-1;
            int x2 = x1-1;

            if(count == 2) makeOperation(x1, x2);
    }

    void popType()
    {
        stackExpression.erase(stackExpression.end());
    }

    void refreshStack(std::string typeResult) throw()
    {
            popType();
            popType();
            pushType(typeResult);
    }

    void makeOperation(int x1, int x2) throw()
    {
            if(stackExpression.at(x1) == ("integer") && stackExpression.at(x2) == ("integer")) {
                if(getLastOperation() != ("relational")) {
                    refreshStack("integer");
                } else {
                    refreshStack("boolean");
                }

                popOperation();
            }
            else if(stackExpression.at(x1) == ("integer") && stackExpression.at(x2) == ("real")) {
                if(getLastOperation() != ("relational")) {
                    refreshStack("real");
                } else {
                    refreshStack("boolean");
                }
                popOperation();
            }
            else if(stackExpression.at(x1) == ("real") && stackExpression.at(x2) == ("integer")) {
                if(getLastOperation() != ("relational")) {
                    refreshStack("real");
                } else {
                    refreshStack("boolean");
                }
                popOperation();
            }
            else if(stackExpression.at(x1) == ("real") && stackExpression.at(2) == ("real")) {
                if(getLastOperation() != ("relational")) {
                    refreshStack("real");
                } else {
                    refreshStack("boolean");
                }
                popOperation();
            }
            else if(stackExpression.at(x1) == ("boolean") && stackExpression.at(x2) == ("boolean")) {
                if(getLastOperation() == ("relational")) {
                    refreshStack("boolean");
                }
                else {
                    std::string par = "Error in operation";
                    throw SemanticException(par);
                }
                popOperation();
            }
            else {
                popOperation();
                std::string par = "Error in types combination!";
                throw SemanticException(par);
            }
    }

    void verifyResult(std::string &typeVar) throw()
    {

            if(stackExpression.at(0) == ("integer") && typeVar == ("integer")) {
                stackExpression.clear();
            }
            else if(stackExpression.at(0) == ("integer") && typeVar == ("real")) {
                stackExpression.clear();
            }
            else if(stackExpression.at(0) == ("real") && typeVar == ("real")) {
                stackExpression.clear();
            }
            else if(stackExpression.at(0) == ("boolean") && typeVar == ("boolean")) {
                stackExpression.clear();
            }
            else {

                stackExpression.clear();
                std::string par = "Error in assigning value to a variable!";
                throw SemanticException(par);
            }
    }

    void reset()
    {
        stackExpression.clear();
    }

    void pushOperation(std::string &operation) {
        operationStack.push_back(operation);
    }

    void popOperation() {
        operationStack.erase(operationStack.end());
    }

    std::string getLastOperation() {
        return operationStack.at(operationStack.size()-1);
    }

    void setCallProcedure(bool b, Symbol &procedureSymbol) {
        callProcedure = b;
        this->procedureSymbol = &procedureSymbol;
    }

    bool isCallProcedure() {
        return callProcedure;
    }

    void pushParameter(std::string type) {
        procedureParametersStack.push_back(type);
    }

    std::string getFirstType() {
        return stackExpression.at(0);
    }

    void resetProcedureControl() {
        procedureParametersStack.clear();
        stackExpression.clear();
        callProcedure = false;
        this->procedureSymbol = nullptr;
    }

    void verifyResultProcedureCall() throw()
    {
            if(procedureSymbol->getParametersSize() !=  procedureParametersStack.size())
            {

                std::string par = "Parameters number error!";
                throw SemanticException(par);

            } else
            {
                for (int i = 0; i < procedureParametersStack.size(); i++) {
                    if (procedureParametersStack.at(i) == ("integer") && procedureSymbol->getParamater(i).getType() == ("integer")) {

                    } else if (procedureParametersStack.at(i) == ("integer") && procedureSymbol->getParamater(i).getType() == ("real")) {

                    } else if (procedureParametersStack.at(i) == ("real") && procedureSymbol->getParamater(i).getType() == ("real")) {

                    }else if (procedureParametersStack.at(i) == ("boolean") && procedureSymbol->getParamater(i).getType() == ("boolean")) {

                    } else {
                        std::string par = "Error in assigning value to parameter";
                        throw SemanticException(par);
                    }
                }
            }

            resetProcedureControl();
    }

};
#endif //PASCALANAYZER_TYPECONTROL_H
