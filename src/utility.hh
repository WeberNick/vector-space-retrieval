/**
 *	@file 	utility.hh
 *	@author	Alexander Weiß, Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de), Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *	@brief  Captures Utility functionality for String operations, IR related operations and similarity measures
 *          Namespace Nesting:
 *          
 *          namespace Utility {
 *              namespace StringOp { ... }
 *              namespace IR { ... }
 *              namespace SimilarityMeasures { ... }
 *          }
 * 
 *	@bugs 	Currently no bugs known
 *	@todos	Write DESCRIPTION
 *
 *	@section DESCRIPTION
 *	TODO
 */
#pragma once

#include "posting_list.hh"
#include "document_manager.hh"
#include "exception.hh"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <algorithm>
#include <bits/stl_algo.h>
#include <cmath>
#include <functional>
#include <iterator>
#include <random>
#include <set>
#include <sstream>
#include <stemming/english_stem.h>
#include <string>

/**
 * @brief Namespace for general utilities
 */
namespace Utility {

    inline double rand_normal(double mean, double stddev) { // Box muller method
        static double n2 = 0.0;
        static int n2_cached = 0;
        if (!n2_cached) {
            double x, y, r;
            do {
                x = 2.0 * rand() / RAND_MAX - 1;
                y = 2.0 * rand() / RAND_MAX - 1;

                r = x * x + y * y;
            } while (r == 0.0 || r > 1.0);
            {
                double d = sqrt(-2.0 * log(r) / r);
                double n1 = x * d;
                n2 = y * d;
                double result = n1 * stddev + mean;
                n2_cached = 1;
                return result;
            }
        } else {
            n2_cached = 0;
            return n2 * stddev + mean;
        }
    }

    /**
     * @brief Generates a random vector of floats with the given dimension and number in the range of min - max. Primarily used for testing
     *
     * @param dimension dimension of the created vector
     * @param min minimum number value
     * @param max maximum number value
     * @return
     */
    inline std::vector<float> generateRandomVector(int dimension, int min, int max) {
        std::random_device rnd_device;
        // Specify the engine and distribution.
        std::mt19937 mersenne_engine(rnd_device());
        std::uniform_real_distribution<float> dist(min, max);

        auto gen = std::bind(dist, mersenne_engine);
        std::vector<float> vec(static_cast<unsigned long>(dimension));
        generate(begin(vec), end(vec), gen);
        return vec;
    }

    inline std::vector<float> generateRandomVectorN(size_t dimension) {
        std::vector<float> result(dimension);
        for (size_t i = 0; i < dimension; ++i) {
            result[i] = static_cast<float>(rand_normal(0, 1));
        }
        return result;
    }

    inline double scalar_product(std::vector<float> const& a, std::vector<float> const& b) {
        if (a.size() != b.size()) { throw std::runtime_error("different sizes"); }
        return std::inner_product(a.begin(), a.end(), b.begin(), 0.0);
    }

    /**
     * @brief Namespace for string operations
     */
    namespace StringOp {
        /**
         * @brief Splits a given string by a delimiter and fills the given string vector
         *
         * @param str the input string to split
         * @param splitChar the delimiter character indicating where to split
         * @param out the vector storing the result
         */
        inline void splitStringBy(const std::string& str, const char splitChar, string_vt& out) {
            size_t found;
            size_t pos = 0;
            while ((found = str.find_first_of(splitChar, pos)) != std::string::npos) {
                out.emplace_back(str.substr(pos, found - pos));
                pos = found + 1;
            }
            out.emplace_back(str.substr(pos));
        }

        /*
         * @brief Splits a given string with the given delimiter. Wrapper function for call to boost
         *
         * @param aString the input string to split
         * @param aDelimiter the delimiter used for splitting
         * @param aOutputVector the vector to store the string tokens in
         * @return -
         */
        inline void splitString(const std::string& aString, const char aDelimiter, string_vt& aOutputVector) {
            boost::split(aOutputVector, aString, boost::is_any_of(std::string(1, aDelimiter)));
        }

        /**
         * @brief Checks whether a given string ends with a specified suffix. Wrapper function for call to boost
         *
         * @param aString the input string
         * @param aSuffix the suffix
         * @return true if input string ends with specified suffix, false otherwise
         */
        inline bool endsWith(const std::string& aString, const std::string& aSuffix) {
            return boost::algorithm::ends_with(aString, aSuffix);
            //            return aString.size() >= aSuffix.size() && 0 == aString.compare(aString.size() - aSuffix.size(), aSuffix.size(), aSuffix);
        }

        /**
         * @brief Lower case a given string
         *
         * @param string string to lower case
         * @return lowercased string
         */
        inline std::string toLower(const std::string& string) {
            std::string data = string;
            std::transform(data.begin(), data.end(), data.begin(), ::tolower);
            return data;
        }

        /**
         * @brief Converts std:string into std:wstring
         *
         * @author
         * https://social.msdn.microsoft.com/Forums/en-US/0f749fd8-8a43-4580-b54b-fbf964d68375/convert-stdstring-to-lpcwstr-best-way-in-c?forum=Vsexpressvc#5d7129ce-73a9-48cc-a818-92e1de4dee9
         * @param s String to convert
         * @return
         */
        inline std::wstring string2wstring(const std::string& s) {
            std::wstring ws;
            ws.assign(s.begin(), s.end());
            return ws;
        }

        /**
         * @brief Converts std:wstring into std:string
         *
         * @author
         * https://social.msdn.microsoft.com/Forums/en-US/0f749fd8-8a43-4580-b54b-fbf964d68375/convert-stdstring-to-lpcwstr-best-way-in-c?forum=Vsexpressvc#5d7129ce-73a9-48cc-a818-92e1de4dee9
         * @param s String to convert
         * @return
         */
        inline std::string wstring2string(const std::wstring& ws) {
            std::string s;
            s.assign(ws.begin(), ws.end());
            return s;
        }

        // TODO: TEST FAILS
        /**
         * @brief Calculates the appearance of a single word inside a string
         *
         * @param str the sentence to check for the word
         * @param word the word to
         * @param case_insensitive delare if the search should be case insensitive or not
         * @return the number of occurences
         */
        inline long countWordInString(std::string str, std::string word, bool case_insensitive) {
            if (case_insensitive) {
                std::string word = toLower(word);
                std::string str = toLower(str);
            }
            std::vector<std::string> content;
            splitString(str, ' ', content);
            size_t count = 0;
            for (size_t i = 0; i < content.size(); ++i) {
                if (content[i] == word) ++count;
            }
            return count;
        }

        /**
         * @brief Calculates the appearance of a single word inside a string
         *
         * @param str the sentence to check for the word
         * @param word the word to count
         * @param case_insensitive delare if the search should be case insensitive or not
         * @return the number of occurences
         */
        inline long countWordInString(std::vector<std::string> str, std::string word, bool case_insensitive) {
            if (case_insensitive) {
                std::string word = toLower(word);
                std::string str = toLower(str);
            }
            size_t count = 0;
            for (const auto& i : str) {
                if (i == word) ++count;
            }
            return count;
        }

        /**
         * @brief Returns the frequency of the most frequent term in the document
         *
         * @param str
         * @return
         */
        inline int getMaxWordFrequency(const std::string& str) {
            std::istringstream input(str);
            std::map<std::string, int> count;
            std::string word;
            decltype(count)::const_iterator most_common;
            while (input >> word) {
                auto iterator = count.emplace(word, 0).first;
                ++iterator->second;
                if (count.size() == 1 || iterator->second > most_common->second) most_common = iterator;
            }
            return most_common->second;
        }

        /**
         * @brief Returns the frequency of the most frequent term in the document
         *
         * @param str
         * @return
         */
        inline int getMaxWordFrequency(std::vector<std::string> str) {
            std::map<std::string, int> count;
            std::string word;

            for (const auto& i : str) {
                // std::cout << i << std::endl;
                count[i]++;
                // std::cout << count[i] << std::endl;
            }

            int maxn = max_element(count.begin(), count.end(),
                                   [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                                       //                    std::cout << a.second << " > " << b.second << std::endl;
                                       return a.second < b.second;
                                   })
                           ->second;

            return maxn;
        }

        /**
         * @brief Trims the left side of a string in place
         *
         * @param s
         */
        static inline void ltrim(std::string& s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
        } // namespace utility

        /**
         * @brief Trims the right side of a string in place
         *
         * @param s
         */
        static inline void rtrim(std::string& s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
        }

        /**
         * @brief Trims the left and right side of a string in place
         *
         * @param s
         */
        static inline void trim(std::string& s) {
            ltrim(s);
            rtrim(s);
        }

        /**
         * @brief Trims the left side of a string
         *
         * @param s
         * @return
         */
        static inline std::string ltrim_copy(std::string s) {
            ltrim(s);
            return s;
        }

        /**
         * @brief Trims the right side of a string
         *
         * @param s
         * @return
         */
        static inline std::string rtrim_copy(std::string s) {
            rtrim(s);
            return s;
        }

        /**
         * @brief Trims the left and right side of a string
         *
         * @param s
         * @return
         */
        static inline std::string trim_copy(std::string s) {
            trim(s);
            return s;
        }
    } // namespace StringOp

    /**
     * @brief Namespace for everything IR related
     */
    namespace IR {
        /**
         * @brief Calculates the term frequency of a given term inside a given string
         *
         * @param term the term to calculate the frequency of
         * @param content the content string in which the term appears
         * @return the term frequency
         */
        inline float calcTF(const std::string& term, const std::string& content) {
            return static_cast<float>((1 + log10(Utility::StringOp::countWordInString(content, term, false))) /
                                      (1 + log10(Utility::StringOp::getMaxWordFrequency(content))));
        }

        /**
         * @brief Calculates the term frequency of a given term inside a given string vector
         *
         * @param term the term to calculate the frequency of
         * @param content The content vector of terms
         * @return the term frequency
         */
        inline float calcTF(const std::string& term, const string_vt& content) {
            return static_cast<float>((1 + log10(Utility::StringOp::countWordInString(content, term, false))) /
                                      (1 + log10(Utility::StringOp::getMaxWordFrequency(content))));
        }

        /**
         * @brief Calculates the tf-idf value
         *
         * @param tf the term frequency
         * @param idf  the inverse document frequency
         * @return the idf value
         */
        inline float calcTF_IDF(const float& tf, const float& idf) { return tf * idf; }

        /**
         * @brief Stems a string word based on the porter stemmer algorithm. Uses https://github.com/OleanderSoftware/OleanderStemmingLibrary
         *
         * @param sentence
         * @return
         */
        inline std::string stemPorter(const std::string& sentence) {
            std::istringstream iss(sentence);
            std::ostringstream os;
            std::string word;

            stemming::english_stem<> StemEnglish;
            while (iss >> word) {
                std::wstring wordToStem = StringOp::string2wstring(word);
                StemEnglish(wordToStem);
                os << StringOp::wstring2string(wordToStem) << " ";
            }
            std::string stemmed = os.str();
            StringOp::trim(stemmed);
            return stemmed;
        }

    } // namespace IR

    /**
     * @brief Namespace for similiarity measures
     */
    namespace SimilarityMeasures {

        inline float calcCosSimEfficient() {
            // TODO
            return 0;
        };

        /**
         * @brief Wrapper method for \link Utility#StringOp#calcCosSim() calcCosSim() \endlink which accepts documents instead of the raw vector
         *
         * @param doc_a
         * @param doc_b
         * @return the cosine similarity
         */
        inline float calcCosSim(const Document& doc_a, const Document& doc_b) {
            // TODO:
            return 0.0; // calcCosSim(doc_a.getTF_IDF(), doc_b.getTF_IDF())
        }

        /**
         * @brief Calculates the cosine similarity between \a aTF_IDF_a and \a aTF_IDF_b
         *
         * @param aTF_IDF_a a tf-idf vector
         * @param aTF_IDF_b a tf-idf vector
         * @return the cosine similarity
         */
        inline float calcCosSim(const std::vector<float>& aTF_IDF_a, const std::vector<float>& aTF_IDF_b) {

            if (aTF_IDF_a.size() != aTF_IDF_b.size()) throw VectorException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Vectors do not have the same size");

            double dotProduct = 0;
            double magnitudeDoc_a = 0;
            double magnitudeDoc_b = 0;

            for (size_t i = 0; i < aTF_IDF_a.size(); ++i) {
                dotProduct += (aTF_IDF_a[i] * aTF_IDF_b[i]);
                magnitudeDoc_a += pow(aTF_IDF_a[i], 2);
                magnitudeDoc_b += pow(aTF_IDF_b[i], 2);
            }

            return static_cast<float>(dotProduct / (sqrt(magnitudeDoc_a) * sqrt(magnitudeDoc_b)));
        }

        /**
         * Returns the angular similarity between two docs
         *
         * @param doc_a
         * @param doc_b
         * @return
         */
        inline float calcAngularSimilarity(const Document& doc_a, const Document& doc_b) {
            return 0.0; // return calcAngularSimilarity(doc_a.getTF_IDF, doc_a.getTF_IDF);
        }

        /**
         * Returns the angular similarity between two docs
         *
         * @see https://en.wikipedia.org/wiki/Cosine_similarity#Angular_distance_and_similarity
         * @param aTF_IDF_a a tf-idf vector
         * @param aTF_IDF_a a tf-idf vector
         * @return
         */
        inline float calcAngularSimilarity(const std::vector<float>& aTF_IDF_a, const std::vector<float>& aTF_IDF_b) {
            float cosine = calcCosSim(aTF_IDF_a, aTF_IDF_b);
            float theta = acosf(cosine);

            return static_cast<float>(1 - (theta / M_PI));
        }

        inline unsigned int calcHammingDist(std::vector<bool>& vec_a, std::vector<bool>& vec_b) {
            float dist = 0;
            for (size_t i = 0; i < vec_a.size(); ++i) {
                if (vec_a[i] != vec_b[i]) { dist++; }
            }
            return dist;
        }

        /**
         * Calculates the cosine similarity of two LSH vectors
         *
         * @see https://stackoverflow.com/questions/12952729/how-to-understand-locality-sensitive-hashing
         * @param vec_a
         * @param vec_b
         * @return
         */
        inline float calcAngSimHamming(std::vector<bool>& vec_a, std::vector<bool>& vec_b) {
            int hamming = calcHammingDist(vec_a, vec_b);
            std::cout << "Hamming: " << hamming << std::endl;
            std::cout << "Vec size: " << vec_a.size() << std::endl;

            double result = cos(hamming / vec_a.size() * 3.14);
            std::cout << result << std::endl;

            float theta = acosf(cos(((hamming / vec_a.size()) * M_PI)));
            std::cout << theta << std::endl;

            return static_cast<float>(1 - (theta / M_PI));
        }

        /**
         * @brief Calculates the euclidean distance between \a doc_a and \a doc_b
         *
         * @param doc_a a document
         * @param doc_b a document
         * @return the euclidean distance as double
         */
        inline float calcEuclDist(const std::vector<float>& doc_a, const std::vector<float>& doc_b) {

            if (doc_a.size() != doc_b.size()) throw VectorException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Vectors do not have the same size");

            double sum = 0;
            for (size_t i = 0; i < doc_a.size(); ++i) {
                sum += pow((doc_a[i] - doc_b[i]), 2);
            }
            return static_cast<float>(sqrt(sum));
        }

        /**
         * @brief Calculates the normalized euclidean distance between \a doc_a and \a doc_b
         *
         * @param doc_a a document
         * @param doc_b a document
         * @return the euclidean distance as double
         */
        inline float calcEuclDistNormalized(std::vector<float> doc_a, std::vector<float> doc_b) {

            if (doc_a.size() != doc_b.size()) throw VectorException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Vectors do not have the same size");

            double magnitudeDoc_a = 0;
            double magnitudeDoc_b = 0;

            for (size_t i = 0; i < doc_a.size(); ++i) {
                magnitudeDoc_a += pow(doc_a[i], 2);
                magnitudeDoc_b += pow(doc_b[i], 2);
            }

            magnitudeDoc_a = sqrt(magnitudeDoc_a);
            magnitudeDoc_b = sqrt(magnitudeDoc_b);

            for (size_t j = 0; j < doc_a.size(); ++j) {
                doc_a[j] = doc_a[j] / magnitudeDoc_a;
                doc_b[j] = doc_b[j] / magnitudeDoc_b;
            }

            return calcEuclDist(doc_a, doc_b);
        }

    } // namespace SimilarityMeasures
} // namespace Utility
