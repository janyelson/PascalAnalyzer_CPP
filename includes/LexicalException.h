//
// Created by MEU PC on 12/11/2017.
//

#ifndef PASCALANAYZER_LEXICALEXCEPTION_H
#define PASCALANAYZER_LEXICALEXCEPTION_H

#include <exception>
#include <string>

class LexicalException : public std::exception
{
    std::string message;
public:
    LexicalException(std::string &message)
    {
        this->message = message;
    }

    LexicalException() : LexicalException((std::string &) "Lexical Error") {}

    const char * what () const throw () override {
        return message.c_str();
    }
};

#endif //PASCALANAYZER_LEXICALEXCEPTION_H
