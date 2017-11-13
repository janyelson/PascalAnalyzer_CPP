//
// Created by MEU PC on 12/11/2017.
//

#ifndef PASCALANAYZER_SEMANTICEXCEPTION_H
#define PASCALANAYZER_SEMANTICEXCEPTION_H

#include <exception>
#include <string>

class SemanticException : public std::exception
{
    std::string message;
public:
    explicit SemanticException(std::string &message)
    {
        this->message = message;
    }

    SemanticException() : SemanticException((std::string &) "Semantic Error") {}

    const char * what () const throw () override {
        return message.c_str();
    }
};
#endif //PASCALANAYZER_SEMANTICEXCEPTION_H
