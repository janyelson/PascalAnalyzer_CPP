//
// Created by MEU PC on 12/11/2017.
//

#ifndef PASCALANAYZER_SYNTAXEXCEPTION_H
#define PASCALANAYZER_SYNTAXEXCEPTION_H

#include <exception>
#include <string>

class SyntaxException : public std::exception
{
    std::string message;
public:
    explicit SyntaxException (std::string &message)
    {
        this->message = message;
    }

    SyntaxException () : SyntaxException ((std::string &) "Syntax Error") {}

    const char * what () const throw () override {
        return message.c_str();
    }
};

#endif //PASCALANAYZER_SYNTAXEXCEPTION_H
