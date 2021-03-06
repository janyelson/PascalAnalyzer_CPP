
#include <string>
#include <vector>
#include "Tokenizer.h"
#include <algorithm>
#include <regex>

void Tokenizer::parser()
{
    try
    {
        for (const std::string &line : lines)
        {
            if (line.empty())
            {
                lineNum += 1;
                continue;
            }
            parseLine(line);
            lineNum += 1;
        }
        //Detecting unclosed comments or strings
        if (onComment)
        {
            lexicalError("A comment was not closed.\nLine: " + (lastOpenCommentLine + 1));
        }

        if (onString)
        {
            lexicalError("A string was not closed.\nLine: " + (lastOpenStringLine + 1));
        }
    }
    catch (LexicalException &lexExc)
    {
        lexExc.what();
    }
    catch (std::exception &ex)
    {
        ex.what();
    }
}

void Tokenizer::parseLine(const std::string &line)
{
    sb = "";
    unsigned long long int length = line.size();

    for (unsigned int i = 0; i < length; i++)
    {
        //Comments
        if ((!onComment && line.at(i) == '{') || onComment)
        {
            onComment = true;
            i += parseComment(line.substr(i, length));
            if (onComment) continue;
        }

        //Comment line
        if (line.at(i) == '/' && su.nextCharEquals(line, i, '/')) return;

        //Strings
        if ((!onString && line.at(i) == '\'') || onString)
        {
            onString = true;
            i += parseString(line.substr(i, length));
            if (onString) continue;
        }

        //Words
        if (su.isLetter(line.at(i)))
        {
            i += parseWord(line.substr(i, length));
            continue;
        }

        //Numbers
        if (su.isDigit(line.at(i)) || (line.at(i) == '.'))
        {
            i += parseNum(line.substr(i, length));
            continue;
        }

        //Extras
        //All specialChars except for ".{}", as they were parsed before
        if (Token::specialChars.find(line.at(i)) != -1)
        {
            i += parseExtras(line.substr(i, length));
            continue;
        }

        //Checking not allowed chars
        if (Token::accChars.find(line.at(i)) == -1)
        {
            std::string errorMsg = "Character not allowed : " + std::string(1, line.at(i)) + "\nLine: " + std::to_string(lastOpenCommentLine + 1);

            if (!DEBUG_MODE) throw LexicalException(errorMsg);

            std::cout << errorMsg << std::endl;
        }
    }
}

int Tokenizer::parseComment(const std::string &substring)
{
    lastOpenCommentLine = lineNum;
    auto length = static_cast<unsigned int>(substring.length());

    for (unsigned int i = 0; i < length; i++)
    {
        if (substring.at(i) == '}')
        {
            onComment = false;
            return i;
        }
    }

    return length - 1;
}

int Tokenizer::parseString(const std::string &substring)
{
    lastOpenStringLine = lineNum;
    auto length = static_cast<int>(substring.length());

    for (unsigned int i = 1; i < length; i++)
    {
        if (substring.at(i) == '\'')
        {
            onString = false;
            return i;
        }
    }

    return length - 1;
}

int Tokenizer::parseWord(const std::string &substring)
{
    sb = "";
    unsigned int i = 0;
    auto length = static_cast<unsigned int>(substring.length());

    //Extraction
    for (; i < length; i++)
    {
        if (!(su.isLetterOrDigit(substring.at(i)) || substring.at(i) == '_')) break;
        sb += substring.at(i);
    }

    std::transform(sb.begin(), sb.end(), sb.begin(), ::tolower);

    //Classification
    if (containsKeyword(sb))
    {
        tokens.emplace_back(sb, Token::Classifications::KEYWORD, lineNum);
    }
    else if (containsBooleanValues(sb))
    {
        tokens.emplace_back(sb, Token::Classifications::BOOLEAN, lineNum);
    }
    else if (sb == (Token::additionOperator))
    {
        tokens.emplace_back(sb, Token::Classifications::ADDITION, lineNum);
    }
    else if (sb == Token::multiplicationOperator)
    {
        tokens.emplace_back(sb, Token::Classifications::MULTIPLICATION, lineNum);
    }
    else
    {
        tokens.emplace_back(sb, Token::Classifications::IDENTIFIER, lineNum);
    }

    return (i == length) ? length - 1 : (i - 1);
}

int Tokenizer::parseNum(const std::string &substring)
{
    //Regex
    //INTEGER
    std::string sInt = "[[:digit:]]+";
    std::regex regInt(sInt);
    //REAL
    std::string sReal = sInt + "\\.[[:digit:]]*";
    std::regex regReal(sReal);

    std::smatch match;

    //COMPLEX
    std::regex regComp("(" + sReal + "|" + sInt + ")i(\\+|-)(" + sReal + "|" + sInt + ")");

    //Starts with a dot but subsequent characters are not a number. It's treated as a DELIMITER dot
    if (!std::regex_search(substring, match, regComp) && !std::regex_search(substring, match, regReal) && !std::regex_search(substring, match, regInt))
    {
        tokens.emplace_back(".", Token::Classifications::DELIMITER, lineNum);
        return 0;
    }

    //If matches, captures the matching string into a token and returns its length
    if (std::regex_search(substring, match, regComp)) {
        std::string search = match.str(0);
        std::string sub = substring.substr(0, search.size());

        if (substring.substr(0, search.size()) == search)
        {
            tokens.emplace_back(search, Token::Classifications::COMPLEX, lineNum);
            return static_cast<unsigned int>(search.size() - 1);
        }
    }

    if (std::regex_search(substring, match, regReal)) {
        std::string search = match.str(0);
        std::string sub = substring.substr(0, search.size());

        if (substring.substr(0, search.size()) == search)
        {
            tokens.emplace_back(search, Token::Classifications::REAL, lineNum);
            return static_cast<unsigned int>(search.size() - 1);
        }
    }

    if (std::regex_search(substring, match, regInt))
    {
        std::string search = match.str(0);

        std::string sub = substring.substr(0, search.size());


        if (substring.substr(0, search.size()) == search) {

            tokens.emplace_back(search, Token::Classifications::INTEGER, lineNum);
            return static_cast<unsigned int>(search.size() - 1);
        }
    }

        //In case some unexpected input is read.
    std::string errorMsg = "Unexpected input: " + substring + "\nline " + std::to_string(lineNum + 1);
    if (!DEBUG_MODE) throw LexicalException(errorMsg);
    std::cout << errorMsg << std::endl;
    return 0;
}

int Tokenizer::parseExtras(const std::string &substring)
{
    sb = "";
    unsigned int i = 0;
    sb += substring.at(0);

    std::string s = ">=";
    //Default classification
    Token::Classifications classification = Token::Classifications::UNCLASSIFIED;

    switch (substring.at(0))
    {
        case '<':
            classification = Token::Classifications::RELATIONAL;
            if (su.nextCharIsIn(substring, i, s))
            {
                sb += (substring.at(++i));
            }
            break;
        case '>':
            classification = Token::Classifications::RELATIONAL;
            if (su.nextCharEquals(substring, i, '='))
            {
                sb += (substring.at(++i));
            }
            break;
        case '=':
            classification = Token::Classifications::RELATIONAL;
            break;
        case ':':
            classification = Token::Classifications::DELIMITER;
            if (su.nextCharEquals(substring, i, '='))
            {
                sb += (substring.at(++i));
                classification = Token::Classifications::ASSIGNMENT;
            }
            break;
        case ',': case ';': case '(': case ')':
            classification = Token::Classifications::DELIMITER;
            break;
        case '+': case '-':
            classification = Token::Classifications::ADDITION;
            break;
        case '*': case '/':
            classification = Token::Classifications::MULTIPLICATION;
            break;
        default:
            break;
    }

    tokens.emplace_back(sb, classification, lineNum);

    return i;
}



