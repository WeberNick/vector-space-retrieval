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

#include "document_manager.hh"
#include "exception.hh"
#include "posting_list.hh"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string_regex.hpp>
#include <boost/regex.hpp>

#include <algorithm>
#include <bits/stl_algo.h>
#include <boost/dynamic_bitset.hpp>
#include <cmath>
#include <functional>
#include <iterator>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <stemming/english_stem.h>
#include <string>
#include <vector>

/**
 * @brief Namespace for general utilities
 */
namespace Utility {

    /**
     * @brief Generates a random normal distributed double number based on the Box muller method
     * @see https://stackoverflow.com/a/28551411
     * @param mean
     * @param stddev
     * @return normal distributed random value
     */
    inline double rand_normal(double mean, double stddev) {
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
    inline std::vector<float> generateRandomVector(size_t dimension, int min, int max) {

        std::random_device rnd_device;
        // Specify the engine and distribution.
        std::mt19937 mersenne_engine(rnd_device());
        std::uniform_int_distribution<int> dist(min, max);

        auto gen = std::bind(dist, std::ref(mersenne_engine));
        std::vector<float> vec(dimension);
        generate(begin(vec), end(vec), gen);
        return vec;
    }

    /**
     * @brief Generates a random vector of size \dimension with random standard normal distributed values
     *
     * @param dimension
     * @return
     */
    inline std::vector<float> generateRandomVectorN(size_t dimension) {
        std::vector<float> result(dimension);
        for (size_t i = 0; i < dimension; ++i) {
            result[i] = static_cast<float>(rand_normal(0, 1));
        }
        return result;
    }

    /**
     * @brief Calculates the dot product of two vectors
     *
     * @param a
     * @param b
     * @return
     */
    template <typename T>
    inline double scalar_product(std::vector<T> const& a, std::vector<T> const& b) {
        if (a.size() != b.size()) throw VectorException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Vectors are not the same size");
        return std::inner_product(a.begin(), a.end(), b.begin(), 0.0);
    }

    inline bool randomProjectionHash(std::vector<float>& origVec, std::vector<float>& randVec) {
        if (origVec.size() != randVec.size()) throw VectorException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Vectors are not the same size");

        double dot = scalar_product(origVec, randVec);
        return dot >= 0;
    }

    template <typename T>
    inline T pop_front(std::vector<T>& vec) {
        assert(!vec.empty());
        T elem = *vec.begin();
        vec.erase(vec.begin());
        return elem;
    }

    /**
     * @brief Namespace for string operations
     */
    namespace StringOp {

        /**
         * @brief Joins a string vector into a normal std::string. Entries in \string_vt are separated by whitespace
         *
         * @param stringVector
         * @return std::string
         */
        inline std::string string_vt_2_str(const string_vt& stringVector) {
            std::string str;

            for (size_t j = 0; j < stringVector.size() - 1; ++j) {
                str += stringVector[j] + " ";
            }
            str += stringVector[stringVector.size() - 1];
            return str;
        }

        /**
         * @brief Splits a given string by a delimiter and fills the given string vector
         *
         * @param str the input string to split
         * @param splitChar the delimiter character indicating where to split
         * @param out the vector storing the result
         */
        inline void splitString(const std::string& str, const char splitChar, string_vt& out) {
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
        inline void splitStringBoost(const std::string& aString, const char aDelimiter, string_vt& aOutputVector) {
            boost::split(aOutputVector, aString, boost::is_any_of(std::string(1, aDelimiter)));
        }

        /**
         * @brief Checks whether a given string ends with a specified suffix. Wrapper function for call to boost
         *
         * @param aString the input string
         * @param aSuffix the suffix
         * @return true if input string ends with specified suffix, false otherwise
         */
        inline bool endsWith(const std::string& aString, const std::string& aSuffix) { return boost::algorithm::ends_with(aString, aSuffix); }

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
         * @brief Lower case a given vector of strings
         *
         * @param string string to lower case
         * @return lowercased string
         */
        inline std::vector<std::string> toLower(const std::vector<std::string>& string) {
            std::vector<std::string> data;
            data.reserve(string.size());
            std::transform(string.begin(), string.end(), std::back_inserter(data), [](const std::string& in) {
                std::string out;
                out.reserve(in.size());
                std::transform(in.begin(), in.end(), std::back_inserter(out), ::tolower);
                return out;
            });
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
         * @param word the word to
         * @param case_insensitive delare if the search should be case insensitive or not
         * @return the number of occurences
         */
        inline long countWordInString(std::string str, std::string word, bool case_insensitive) {
            std::string wordCased = case_insensitive ? toLower(word) : word;
            std::string strCased = case_insensitive ? toLower(str) : str;

            std::vector<std::string> content;
            splitString(strCased, ' ', content);

            size_t count = 0;
            for (size_t i = 0; i < content.size(); ++i) {
                if (content[i] == wordCased) ++count;
            }
            return count;
        }

        /**
         * @brief Calculates the appearance of a single word inside a string vector
         *
         * @param str the sentence to check for the word
         * @param word the word to count
         * @param case_insensitive delare if the search should be case insensitive or not
         * @return the number of occurences
         */
        inline long countWordInString(std::vector<std::string> str, std::string word, bool case_insensitive) {

            std::string word_2 = case_insensitive ? toLower(word) : word;
            std::vector<std::string> str_2 = case_insensitive ? toLower(str) : str;

            size_t count = 0;
            for (const auto& i : str) {
                if (i == word) ++count;
            }
            return count;
        }

        /**
         * @brief Returns the frequency of the most frequent term a string
         *
         * @param str std:string
         * @return
         */
        inline int getMaxWordFrequency(const std::string& str) {
            if (str == "") return 0;
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
         * @brief Returns the frequency of the most frequent term in a string vector
         *
         * @param str std::vector<std::string>
         * @return
         */
        inline int getMaxWordFrequency(std::vector<std::string> vec) {
            if (vec.size() == 0)
                return 0;
            else if (vec.size() == 1)
                return getMaxWordFrequency(vec.at(0));
            else {
                std::map<std::string, int> count;
                std::string word;

                for (const auto& i : vec) {
                    ++count[i];
                }
                int maxn = max_element(count.begin(), count.end(),
                                       [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) { return a.second < b.second; })
                               ->second;
                return maxn;
            }
        }

        /**
         * @brief Removes empty strings from a string vector
         *
         * @param vec std::vector<std::string>
         */
        inline void removeEmptyStringsFromVec(std::vector<std::string>& vec) {
            vec.erase(std::remove_if(vec.begin(), vec.end(), [](const std::string& s) { return s.empty(); }), vec.end());
        }

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

    } // namespace StringOp

    /**
     * @brief Namespace for everything IR related
     */
    namespace IR {

        /**
         * @brief Calculates the term frequency a term
         *
         * @param term count of the term
         * @param max maximum count of another term in a document
         * @return float
         */
        inline float calcTf(const float term, const float max) { return static_cast<float>((1 + log10(term)) / (1 + log10(max))); }

        /**
         * @brief Calculates the IDF of a term
         *
         * @param term
         * @param max
         * @return float
         */
        inline float calcIdf(const float N, const float docs) { return static_cast<float>(log10(N / docs)); }

        /**
         * @brief Calculates the tf-idf value
         *
         * @param tf the term frequency
         * @param idf  the inverse document frequency
         * @return the idf value
         */
        inline float calcTfIdf(const float tf, const float idf) { return tf * idf; }

        /**
         * @brief Removes all stopwords specified in \stopwordList from \str
         *
         * @param str std::string
         * @param stopwordList
         */
        inline void removeStopword(std::string& str, const string_vt& stopwordList) {
            int counter = 0;
            for (auto& elem : stopwordList) {
                std::regex regex("\\b(" + elem + ")\\b");
                while (std::regex_search(str, regex)) {
                    str = std::regex_replace(str, regex, "");
                }
                ++counter;
            }
        }

        /**
         * @brief Calculates the term frequency of a given term inside a given string
         *
         * @param term the term to calculate the frequency of
         * @param content the content string in which the term appears
         * @return the term frequency
         */
        inline float calcTf(const std::string& term, const std::string& content) {
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
        inline float calcTf(const std::string& term, const std::vector<std::string>& content) {
            return static_cast<float>((1 + log10(Utility::StringOp::countWordInString(content, term, false))) /
                                      (1 + log10(Utility::StringOp::getMaxWordFrequency(content))));
        }

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

        // input: (1, 0.5), (2, 0), (3, 0.45), ..
        // output: {(0, <PLObj>: (1, 0.4), (3, 0.8), ..), (1, ..), ..}
        // TODO docs
        /**
         * @brief
         *
         * @param aNumTiers
         * @param aPostingList
         * @return std::map<size_t, PostingList>
         */
        inline std::map<size_t, PostingList> calculateTiers(const size_t aNumTiers, const PostingList& aPostingList) {
            const sizet_float_mt& aPosting = aPostingList.getPosting();
            const float idf = aPostingList.getIdf();
            std::map<size_t, PostingList> outputMap;

            std::vector<std::pair<size_t, float>> vec; // vector of <id, tf> pairs, will be sorted descending by tf
            for (auto it = aPosting.begin(); it != aPosting.end(); ++it) {
                vec.push_back(*it);
            }
            std::sort(vec.begin(), vec.end(), [](auto& a, auto& b) { return a.second > b.second; }); // desc

            size_t size = aPosting.size();
            uint boundary = std::floor((double)size / aNumTiers);
            for (size_t tier = 0; tier < aNumTiers; ++tier) {
                sizet_float_mt posting;
                if (size < aNumTiers && !vec.empty()) {
                    posting.insert(Utility::pop_front(vec));
                } else {
                    boundary = (tier == (aNumTiers - 1)) ? vec.size() : boundary;
                    for (size_t i = 0; i < boundary; ++i) {
                        posting.insert(Utility::pop_front(vec));
                    }
                }
                PostingList postinglist(idf, posting);
                outputMap.insert(std::make_pair(tier, postinglist));
            }
            return outputMap;
        }

        /**
         * @brief
         *
         * @param first
         * @param second
         * @return sizet_vt
         */
        inline void mergePostingLists(const sizet_vt& first, const sizet_vt& second, sizet_vt& out) {
            auto ione = first.begin();
            auto itwo = second.begin();
            out.clear();
            while (ione != first.end() && itwo != second.end()) {

                if (*ione == *itwo) {
                    out.push_back(*ione);
                    ++ione;
                    ++itwo;
                } else if (*ione < *itwo) {
                    ++ione;
                } else
                    ++itwo;
            }
        }

        /**
         * @brief
         *
         * @param vecs
         * @param out
         */
        inline void mergePostingLists(std::vector<sizet_vt>& vecs, sizet_vt& out) {

           if (vecs.size() > 1) {
                std::sort(vecs.begin(), vecs.end(), [](const sizet_vt& a, const sizet_vt& b) { return a.size() < b.size(); }); // asc
                out.clear();
                mergePostingLists(vecs.at(0), vecs.at(1), out);
                if (vecs.size() == 2) return;
                for (size_t i = 2; i < vecs.size(); ++i) {
                    sizet_vt out_copy(out);
                    mergePostingLists(out_copy, vecs.at(i), out);
                }
            } else {
                out = vecs.at(0);
            }
        }
    } // namespace IR

    /**
     * @brief Namespace for similiarity measures
     */
    namespace SimilarityMeasures {

        /**
         * @brief calculate and return the length of the given vector
         *
         * @param vec the vector
         * @return float the length
         */
        inline double vectorLength(const std::vector<float>& vec) {
            double magn = 0;
            for (size_t i = 0; i < vec.size(); ++i) {
                magn += pow(vec[i], 2);
            }
            return sqrt(magn);
        }

        /**
         * @brief Calculates the cosine similarity between \aTfIdf_a and \aTfIdf_b
         *
         * @param aTfIdf_a a tf-idf vector
         * @param aTfIdf_b a tf-idf vector
         * @return the cosine similarity
         */
        inline float calcCosSim(const std::vector<float>& aTfIdf_a, const std::vector<float>& aTfIdf_b) {
            if (aTfIdf_a.size() != aTfIdf_b.size()) throw VectorException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Vectors do not have the same size");

            double dotProduct = 0;
            for (size_t i = 0; i < aTfIdf_a.size(); ++i) {
                dotProduct += (aTfIdf_a[i] * aTfIdf_b[i]);
            }
            return static_cast<float>(dotProduct / (vectorLength(aTfIdf_a) * vectorLength(aTfIdf_b)));
        }

        /**
         * @brief Wrapper method for \link Utility#StringOp#calcCosSim() calcCosSim() \endlink which accepts documents instead of the raw vector
         *
         * @param doc_a
         * @param doc_b
         * @return the cosine similarity
         */
        inline float calcCosSim(const Document& doc_a, const Document& doc_b) { return calcCosSim(doc_a.getTfIdfVector(), doc_b.getTfIdfVector()); }

        /**
         * @brief Calculates the cosine distance of two vectors
         *
         * @param aTF_IDF_a
         * @param aTF_IDF_b
         * @return
         */
        inline float calcCosDist(const std::vector<float>& aTF_IDF_a, const std::vector<float>& aTF_IDF_b) { return 1 - calcCosSim(aTF_IDF_a, aTF_IDF_b); }

        /**
         * @brief Calculates the cosine distance of two documents
         *
         * @param aTF_IDF_a
         * @param aTF_IDF_b
         * @return
         */
        inline float calcCosDist(const Document& doc_a, const Document& doc_b) { return calcCosDist(doc_a.getTfIdfVector(), doc_b.getTfIdfVector()); }

        /**
         * Returns the angular similarity between two docs
         *
         * @see https://en.wikipedia.org/wiki/Cosine_similarity#Angular_distance_and_similarity
         * @param aTfIdf_a a tf-idf vector
         * @param aTfIdf_b a tf-idf vector
         * @return
         */
        inline float calcAngularSimilarity(const std::vector<float>& aTfIdf_a, const std::vector<float>& aTfIdf_b) {

            if (aTfIdf_a.size() != aTfIdf_a.size()) throw VectorException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Vectors do not have the same size.");

            float cosine = calcCosSim(aTfIdf_a, aTfIdf_b);
            float theta = acosf(cosine);

            return static_cast<float>(1 - (theta / M_PI));
        }

        /**
         * Returns the angular similarity between two docs
         *
         * @param doc_a
         * @param doc_b
         * @return
         */
        inline float calcAngularSimilarity(const Document& doc_a, const Document& doc_b) {
            return calcAngularSimilarity(doc_a.getTfIdfVector(), doc_b.getTfIdfVector());
        }

        /**
         * @brief Calculates the Hamming distance between two std::vector<bools> vectors
         *
         * @param vec_a
         * @param vec_b
         * @return
         */
        inline unsigned int calcHammingDist(std::vector<bool>& vec_a, std::vector<bool>& vec_b) {

            if (vec_a.size() != vec_a.size()) throw VectorException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Vectors do not have the same size.");

            unsigned int dist = 0;
            for (size_t i = 0; i < vec_a.size(); ++i) {
                if (vec_a[i] != vec_b[i]) { dist++; }
            }
            return dist;
        }

        /**
         * @brief Calculates the Hamming distance between two boost::dynamic_bitset sets
         *
         * @param vec_a
         * @param vec_b
         * @return
         */
        inline unsigned int calcHammingDist(const boost::dynamic_bitset<>& vec_a, const boost::dynamic_bitset<>& vec_b) {
            return static_cast<unsigned int>((vec_a ^ vec_b).count());
        }

        /**
         * @brief Calculates the Hamming similarity between two boost::dynamic_bitset sets
         * (\vec_a.size() -  \calcHammingDist(\vec_a, \vec_b))
         *
         * @param vec_a
         * @param vec_b
         * @return
         */
        inline unsigned int calcHammingSim(boost::dynamic_bitset<>& vec_a, boost::dynamic_bitset<>& vec_b) {
            return static_cast<unsigned int>(vec_a.size() - calcHammingDist(vec_a, vec_b));
        }

        /**
         * Calculates the angular similarity of two LSH vectors
         *
         * @see https://stackoverflow.com/questions/12952729/how-to-understand-locality-sensitive-hashing
         * @param vec_a
         * @param vec_b
         * @return
         */
        inline float calcAngSimHamming(std::vector<bool>& vec_a, std::vector<bool>& vec_b) {
            int hamming = calcHammingDist(vec_a, vec_b);

            double theta = acosf(cos(((hamming / vec_a.size()) * M_PI)));

            return static_cast<float>(1 - (theta / M_PI));
        }

        /**
         * @brief Calculates the euclidean distance between \doc_a and \doc_b
         *
         * @param doc_a a document
         * @param doc_b a document
         * @return the euclidean distance as double
         */
        inline float calcEuclDist(const std::vector<float>& doc_a, const std::vector<float>& doc_b) {
            if (doc_a.size() != doc_b.size()) throw VectorException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Vectors do not have the same size.");

            double sum = 0;
            for (size_t i = 0; i < doc_a.size(); ++i) {
                sum += pow((doc_a[i] - doc_b[i]), 2);
            }
            return static_cast<float>(sqrt(sum));
        }

        /**
         * @brief Calculates the normalized euclidean distance between \doc_a and \doc_b
         *
         * @param doc_a a document
         * @param doc_b a document
         * @return the euclidean distance as double
         */
        inline float calcEuclDistNormalized(std::vector<float>& doc_a, std::vector<float>& doc_b) {
            if (doc_a.size() != doc_b.size()) throw VectorException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Vectors do not have the same size.");

            double len_a = vectorLength(doc_a);
            double len_b = vectorLength(doc_b);
            for (size_t j = 0; j < doc_a.size(); ++j) {
                doc_a[j] = doc_a[j] / len_a;
                doc_b[j] = doc_b[j] / len_b;
            }
            return calcEuclDist(doc_a, doc_b);
        }

    } // namespace SimilarityMeasures
} // namespace Utility
