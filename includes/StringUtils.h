#ifndef STRINGUTILS_H_INCLUDED
#define STRINGUTILS_H_INCLUDED

#include <string>

class StringUtils
{

public:

    /**
     * Checks if the index it's the last of the string.
     *
     * @param string A not null String.
     * @param idx    Index for testing.
     * @return true, if idx is greater than or equal to the string length; false, otherwise.
     */
    bool isLastChar(const std::string &str, int idx) {return (idx + 1 >= str.length());}

    /**
     * Checks on the string if the next char, of the specified index, equals to a char.
     *
     * @param string A not null String.
     * @param idx    Index of the CURRENT char.
     * @param c      Char to be tested.
     * @return true, if the idx is valid and not the last of the string and the following char equals to char c.
     */
    bool nextCharEquals(const std::string &str, unsigned int idx, char c) {return !isLastChar(str, idx) && str.at(idx + 1) == c;}


    /**
     * Checks on the string, if the next char equals to any char of another string.
     *
     * @param string A not null String.
     * @param idx    Index of the CURRENT char.
     * @param chars  A not null String containing all chars to be verified.
     * @return true, if the idx is valid and not the last of the string and the following char is in String chars.
     */
    bool nextCharIsIn(const std::string &str, unsigned int idx, std::string &chars)
    {
        return !isLastChar(str, idx) && chars.find(str.at(idx + 1)) != -1;;
    }


    /**
     * Checks if the char is in [a-z] or [A-Z]. It will return false, if char c contains some diacritic.
     *
     * @param c Char to be tested.
     * @return true, if the character is in [a-z] or [A-Z]; false, otherwise.
     */
    bool isLetter(const char c) {return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');}

    /**
     * Checks if the char is one of the ISO-LATIN-1 digits.
     *
     * @param c Char to be tested.
     * @return true, if the char is one of the ISO-LATIN-1 digits; false, otherwise.
     */
    bool isDigit(const char c) {return (c >= '0' && c <= '9');}

    /**
     * Checks if the char is in [a-z] or [A-Z] or is one of the ISO-LATIN-1 digits.
     * It will return false, if char c contains some diacritic.
     *
     * @param c Char to be tested.
     * @return true, if the character is in [a-z] or [A-Z]; false, otherwise.
     */
    bool isLetterOrDigit(const char c) {return isLetter(c) || isDigit(c);}
};

#endif // STRINGUTILS_H_INCLUDED
