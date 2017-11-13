#include "Token.h"

const std::array<std::string, 14> Token::keywords = {"program", "var", "integer", "real", "boolean", "procedure", "begin", "end", "if", "then", "else", "while", "do", "not"};
const std::array<std::string, 2> Token::booleanValues = {"true", "false"};
const std::string Token::additionOperator = "or";
const std::string Token::multiplicationOperator = "and";
const std::string Token::accChars = "_.:;,<>=+-/*(){}\' \t\n";
const std::string Token::specialChars = "_.:;,<>=+-/*(){}";
const std::string Token::specialChars2 = ":;,<>=+-/*()";
const std::array<std::string, 3> Token::types = {"integer", "real", "boolean"};


