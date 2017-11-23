#ifndef TOKENIZER_H_INCLUDED
#define TOKENIZER_H_INCLUDED

#include <vector>
#include <string>
#include <iostream>

#include "Token.h"
#include "LexicalException.h"
#include "StringUtils.h"

class Tokenizer
{
private:

    /*PROPERTIES*/
    std::vector<std::string> lines;
    std::vector<Token> tokens;
    int lineNum;

    bool DEBUG_MODE;
    bool onComment{};
    int lastOpenCommentLine{};
    bool onString{};
    int lastOpenStringLine{};

    /*STRING*/
    std::string sb;
    StringUtils su;

public:

    Tokenizer(std::vector<std::string> &l, bool D) : lines(l), lineNum(0), DEBUG_MODE(D)
    {
        if (lines.empty())
        {
            throw LexicalException((std::string &) "\"lines\" cannot be null.");
        }
    }

    Tokenizer(const Tokenizer &that)
    {
        lines = that.lines;
        tokens = that.tokens;
        lineNum = that.lineNum;

        DEBUG_MODE = that.DEBUG_MODE;
        onComment = that.onComment;
        lastOpenCommentLine = that.lastOpenCommentLine;
        onString = that.onString;
        lastOpenStringLine = that.lastOpenStringLine;

        sb = that.sb;
        su = that.su;
    }

    explicit Tokenizer(std::vector<std::string> &lines) : Tokenizer(lines, false) {}

    std::vector<Token> getTokens()
    {
        return tokens;
    }

    void parser();

private:

    void parseLine(const std::string &line);

    int parseComment(const std::string &substring);

    int parseString(const std::string &substring);

    int parseWord(const std::string &substring);

    int parseNum(const std::string &substring);

    int parseExtras(const std::string &substring);

    bool containsKeyword(const std::string &word) const
    {
        for(const std::string &keyword : Token::keywords)
        {
            if(word == keyword) return true;
        }

        return false;
    }

    bool containsBooleanValues(const std::string &word) const
    {
        for(const std::string &keyword : Token::booleanValues)
        {
            if(word == keyword) return true;
        }

        return false;
    }

    void lexicalError(std::string errorMsg) throw()
    {
        if (!this->DEBUG_MODE)
        {
            throw LexicalException(errorMsg);
        }

        std::cout << errorMsg << std::endl;
    }
};

#endif // TOKENIZER_H_INCLUDED
