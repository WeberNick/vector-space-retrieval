/*********************************************************************
 * @file    string_util.hh
 * @author 	Nick Weber
 * @date    Mai 18, 2018
 * @brief 	
 * @bugs 	TBD
 * @todos 	TBD
 * 
 * @section	DESCRIPTION
 * TBD
 * 
 * @section USE
 * TBD
 ********************************************************************/
#pragma once

#include "types.hh"
#include "trace.hh"
#include "exception.hh"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string_regex.hpp>
#include <boost/regex.hpp>
#include <string>
#include <regex>
#include <vector>
#include <functional>

namespace Util
{
        /**
         * @brief Joins a string vector into a normal std::string. Entries in \string_vt are separated by whitespace
         *
         * @param stringVector
         * @return std::string
         */
        std::string string_vt_2_str(const string_vt& stringVector) ;

        /*
         * @brief Splits a given string with the given delimiter. Wrapper function for call to boost
         *
         * @param aString the input string to split
         * @param aDelimiter the delimiter used for splitting
         * @param aOutputVector the vector to store the string tokens in
         * @return -
         */
        void splitStringBoost(const std::string& aString, const char aDelimiter, string_vt& aOutputVector);

        /**
         * @brief Checks whether a given string ends with a specified suffix. Wrapper function for call to boost
         *
         * @param aString the input string
         * @param aSuffix the suffix
         * @return true if input string ends with specified suffix, false otherwise
         */
        bool endsWith(const std::string& aString, const std::string& aSuffix);

        /**
         * @brief Lower case a given string
         *
         * @param string string to lower case
         * @return lowercased string
         */
        std::string toLower(const std::string& aString);
        std::string& toLower(std::string& aString);

        /**
         * @brief Lower case a given vector of strings
         *
         * @param string string to lower case
         * @return lowercased string
         */
        string_vt toLower(const string_vt& aStrings);
        string_vt& toLower(string_vt& aStrings);

        /**
         * @brief Converts std:string into std:wstring
         *
         * @author
         * https://social.msdn.microsoft.com/Forums/en-US/0f749fd8-8a43-4580-b54b-fbf964d68375/convert-stdstring-to-lpcwstr-best-way-in-c?forum=Vsexpressvc#5d7129ce-73a9-48cc-a818-92e1de4dee9
         * @param s String to convert
         * @return
         */
        std::wstring string2wstring(const std::string& s); 

        /**
         * @brief Converts std:wstring into std:string
         *
         * @author
         * https://social.msdn.microsoft.com/Forums/en-US/0f749fd8-8a43-4580-b54b-fbf964d68375/convert-stdstring-to-lpcwstr-best-way-in-c?forum=Vsexpressvc#5d7129ce-73a9-48cc-a818-92e1de4dee9
         * @param s String to convert
         * @return
         */
        std::string wstring2string(const std::wstring& ws);

        /**
         * @brief Calculates the appearance of a single word inside a string
         *
         * @param str the sentence to check for the word
         * @param word the word to
         * @param case_insensitive delare if the search should be case insensitive or not
         * @return the number of occurences
         */
        size_t countWordInString(const std::string& str, const std::string& word, bool case_insensitive);

        /**
         * @brief Calculates the appearance of a single word inside a string vector
         *
         * @param str the sentence to check for the word
         * @param word the word to count
         * @param case_insensitive delare if the search should be case insensitive or not
         * @return the number of occurences
         */
        size_t countWordInString(const string_vt& str, const std::string& word, bool case_insensitive);

        /**
         * @brief Returns the frequency of the most frequent term a string
         *
         * @param str std:string
         * @return
         */
        int getMaxWordFrequency(const std::string& str);

        /**
         * @brief Returns the frequency of the most frequent term in a string vector
         *
         * @param str std::vector<std::string>
         * @return
         */
        int getMaxWordFrequency(string_vt vec);

        /**
         * @brief Removes empty strings from a string vector
         *
         * @param vec std::vector<std::string>
         */
        void removeEmptyStringsFromVec(string_vt& vec);

        /**
         * @brief Trims the left side of a string in place
         *
         * @param s std::string
         */
        std::string& ltrim(std::string& s);

        /**
         * @brief Trims the right side of a string in place
         *
         * @param s std::string
         */
        std::string& rtrim(std::string& s);

        /**
         * @brief Trims the left and right side of a string in place
         *
         * @param s std::string
         */
        std::string& trim(std::string& s);

        /**
         * @brief Trims the left side of a string
         *
         * @param s std::string
         * @return
         */
        std::string ltrim_copy(const std::string& s); 

        /**
         * @brief Trims the right side of a string
         *
         * @param s std::string
         * @return
         */
        std::string rtrim_copy(const std::string& s);

        /**
         * @brief Trims the left and right side of a string
         *
         * @param s std::string
         * @return
         */
        std::string trim_copy(const std::string& s);

        /**
         * @brief Removes all stopwords specified in \stopwordList from \str
         *
         * @param str std::string
         * @param stopwordList
         */
        void removeStopword(std::string& str, const string_vt& stopwordList); 

        /**
         *
         */
        string_vt preprocess(std::string& aContent, const string_vt& aStopwordsList);

}
