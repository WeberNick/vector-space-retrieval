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
#include <vector>

namespace Util
{
        /**
         * @brief Joins a string vector into a normal std::string. Entries in \string_vt are separated by whitespace
         *
         * @param stringVector
         * @return std::string
         */
        std::string string_vt_2_str(const string_vt& stringVector) ;

        /**
         * @brief Splits a given string by a delimiter and fills the given string vector
         *
         * @param str the input string to split
         * @param splitChar the delimiter character indicating where to split
         * @param out the vector storing the result
         */
        void splitString(const std::string& str, const char splitChar, string_vt& out);

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
        std::string toLower(const std::string& string);

        /**
         * @brief Lower case a given vector of strings
         *
         * @param string string to lower case
         * @return lowercased string
         */
        string_vt toLower(const string_vt& string);

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
        long countWordInString(std::string str, std::string word, bool case_insensitive);

        /**
         * @brief Calculates the appearance of a single word inside a string vector
         *
         * @param str the sentence to check for the word
         * @param word the word to count
         * @param case_insensitive delare if the search should be case insensitive or not
         * @return the number of occurences
         */
        long countWordInString(string_vt str, std::string word, bool case_insensitive);

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
        static inline void ltrim(std::string& s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
        }

        /**
         * @brief Trims the right side of a string in place
         *
         * @param s std::string
         */
        static inline void rtrim(std::string& s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
        }

        /**
         * @brief Trims the left and right side of a string in place
         *
         * @param s std::string
         */
        static inline void trim(std::string& s) {
            ltrim(s);
            rtrim(s);
        }

        /**
         * @brief Trims the left side of a string
         *
         * @param s std::string
         * @return
         */
        static inline std::string ltrim_copy(std::string s) {
            ltrim(s);
            return s;
        }

        /**
         * @brief Trims the right side of a string
         *
         * @param s std::string
         * @return
         */
        static inline std::string rtrim_copy(std::string s) {
            rtrim(s);
            return s;
        }

        /**
         * @brief Trims the left and right side of a string
         *
         * @param s std::string
         * @return
         */
        static inline std::string trim_copy(std::string s) {
            trim(s);
            return s;
        }

}
