//
// Created by Alexander Weiß on 30.03.18.
//

/**
 *  Namespace Nesting explained:
 *
 *  namespace Utility
 *  {
 *      namespace StringOp{...}
 *      namespace IR{...}
 *      namespace SimilarityMeasures{...}
 *  }
 */
#pragma once

#include "document_manager.hh"
#include "exception.hh"
#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <lib/oleanderStemmingLibrary/stemming/english_stem.h>
#include <random>
#include <sstream>
#include <string>

/**
 * @brief Namespace for general utilities
 */
namespace Utility {

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
        std::vector<float> vec(dimension);
        generate(begin(vec), end(vec), gen);
        return vec;
    }

    /**
     * @brief Namespace for string operations
     */
    namespace StringOp {
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

        /**
         * @brief Calculates the appearance of a single word inside a string
         *
         * @param str the sentence to check for the word
         * @param word the word to count
         * @param case_insensitive delare if the search should be case insensitive or not
         * @return the number of occurences
         */

        inline long countWordInString(const std::string& str, const std::string& word, bool case_insensitive) {
            if (case_insensitive) {

                std::string word_lower = toLower(word);
                std::string string_lower = toLower(str);
                std::istringstream ss(string_lower);

                return std::count_if(std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>(),
                                     [word_lower](const std::string& s) { return s == word_lower; });
            } else {
                std::istringstream ss(str);
                return std::count_if(std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>(),
                                     [word](const std::string& s) { return s == word; });
            }
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
         * @brief Calculates the term frequency of a given term inside a given document
         *
         * @param term the term to calculate the frequency of
         * @param content the content string in which the term appears
         * @return the term frequency
         */
        inline float calcTF(const std::string& term, const std::string& content) {

            // TODO: max frequency of any word in the dox instead of the 10 here
            return static_cast<float>((1 + log10(StringOp::countWordInString(content, term, false))) / (1 + log10(10)));
        }

        /**
         * @brief Calculates the inverted document of a given term inside a given document collection
         *
         * @param term the term to calculate the frequency of
         * @param documentManager the document in which the term appears
         * @return the inverted document frequency
         */
        inline float calcIDF(const std::string& term, const DocumentManager& documentManager) {}

        /**
         * @brief Calculates the tf-idf value
         *
         * @param tf the term frequency
         * @param idf  the inverse document frequency
         * @return the idf value
         */
        inline float calcTF_IDF(const float& tf, const float& idf) { return tf * idf; }

        /**
         * @brief Stems a string word based on the porter stemmer algorithm
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

        inline float calcCosSimEfficient(){};

        /**
         * @brief Wrapper method for <calcCosSim>"()" which accepts documents instead of the raw vector
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
         * @brief Calculates the cosine similarity between #aTF_IDF_a and #aTF_IDF_b
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

            for (int i = 0; i < aTF_IDF_a.size(); ++i) {
                dotProduct += (aTF_IDF_a[i] * aTF_IDF_b[i]);
                magnitudeDoc_a += pow(aTF_IDF_a[i], 2);
                magnitudeDoc_b += pow(aTF_IDF_b[i], 2);
            }

            return static_cast<float>(dotProduct / (sqrt(magnitudeDoc_a) * sqrt(magnitudeDoc_b)));
        }

        /**
         * Returns 1 - <calcCosSim>"()". So we have the higher the returned value the more similar the docs are. This is just for negating the
         * counter-intuitive measurement of the cosine similarity for being high for unsimilar documents
         *
         * @param doc_a
         * @param doc_b
         * @return
         */
        inline float calcCosDist(const Document& doc_a, const Document& doc_b) { return 1 - calcCosSim(doc_a, doc_b); }

        /**
         * @brief Calculates the euclidean distance between #doc_a and #doc_b
         *
         * @param doc_a a document
         * @param doc_b a document
         * @return the euclidean distance as double
         */
        inline float calcEuclDist(const std::vector<float>& doc_a, const std::vector<float>& doc_b) {

            if (doc_a.size() != doc_b.size()) throw VectorException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Vectors do not have the same size");

            double sum = 0;
            for (int i = 0; i < doc_a.size(); ++i) {
                sum += pow((doc_a[i] - doc_b[i]), 2);
            }
            return static_cast<float>(sqrt(sum));
        }

        /**
         * @brief Calculates the normalized euclidean distance between #doc_a and #doc_b
         *
         * @param doc_a a document
         * @param doc_b a document
         * @return the euclidean distance as double
         */
        inline float calcEuclDistNormalized(std::vector<float> doc_a, std::vector<float> doc_b) {

            if (doc_a.size() != doc_b.size()) throw VectorException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Vectors do not have the same size");

            double magnitudeDoc_a = 0;
            double magnitudeDoc_b = 0;

            for (int i = 0; i < doc_a.size(); ++i) {
                magnitudeDoc_a += pow(doc_a[i], 2);
                magnitudeDoc_b += pow(doc_b[i], 2);
            }

            magnitudeDoc_a = sqrt(magnitudeDoc_a);
            magnitudeDoc_b = sqrt(magnitudeDoc_b);

            for (int j = 0; j < doc_a.size(); ++j) {
                doc_a[j] = doc_a[j] / magnitudeDoc_a;
                doc_b[j] = doc_b[j] / magnitudeDoc_b;
            }

            return calcEuclDist(doc_a, doc_b);
        }

    } // namespace SimilarityMeasures
} // namespace Utility
