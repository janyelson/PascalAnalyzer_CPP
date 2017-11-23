#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

#include <string>
#include <array>
#include <utility>

class Token
{
private:

    /*PROPERTIES*/
    std::string text;
    int classification;
    int lineNumber;

public:
    /*VALUES*/
    const static std::array<std::string, 14> keywords;
    const static std::array<std::string, 2> booleanValues;
    const static std::string additionOperator;
    const static std::string multiplicationOperator;
    const static std::string accChars;
    const static std::string specialChars;
    const static std::array<std::string, 3> types;

    Token() : Token (std::string("")) {};

    Token(const Token &that)
    {
        text = that.text;
        classification = that.classification;
        lineNumber = that.lineNumber;
    }

    explicit Token(const std::string &text) : Token (text, Classifications::UNCLASSIFIED) {}

    Token(const std::string &text, int classification) : Token (text, classification, 0) {}

    Token (std::string t, int c, int l) : text(std::move(t)), classification(c), lineNumber(l) {}

    std::string toString()
    {
        std::string ret = "TOKEN: " + text + "\n"
                          + "CLASSIFICATION: " + getClassificationName() + "\n"
                          + "LINE: " + std::to_string(lineNumber);
        return ret;
    }
    std::string & getText() {return this->text;}
    int getLineNumber() {return this->lineNumber;}

    /**
     * All possible classifications for tokens in Pascal
     */
    enum Classifications
    {
        KEYWORD = 1, //any word in keywords list
        IDENTIFIER = 2, //[a..z] ([a..z]|[0..9]|'_')*
        INTEGER = 3, //[0..9]+
        REAL = 4, //[0..9]*.[0..9]+
        BOOLEAN = 5, //true false
        COMPLEX = 6, //[REAL|INTEGER]i[+|-]?[REAL|INTEGER]
        DELIMITER = 7, //,;:.()
        ASSIGNMENT = 8, //:=
        RELATIONAL = 9, //<,>,=,<>,<=,>=
        ADDITION = 10, //or + -
        MULTIPLICATION = 11, //and * /
        UNCLASSIFIED = 12 //default value
    };

    int getClassification()
    {
        return classification;
    }

    std::string getClassificationName()
    {
        switch(classification)
        {
            case 1: return "keyword";
            case 2: return "identifier";
            case 3: return "integer";
            case 4: return "real";
            case 5: return "boolean";
            case 6: return "complex";
            case 7: return "delimiter";
            case 8: return "assignment";
            case 9: return "relational";
            case 10: return "addition";
            case 11: return "multiplication";
            case 12: return "unclassified";
            default: return "unclassified";
        }
    }
};

#endif // TOKEN_H_INCLUDED
