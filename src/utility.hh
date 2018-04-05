//
// Created by Alexander Weiß on 30.03.18.
//
#pragma once

#include "document_manager.hh"
#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <random>
#include <sstream>
#include <string>
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
        return std::count_if(std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>(), [word](const std::string& s) { return s == word; });
    }
}

/**
 * @brief Generates a random vector of doubles with the given dimension and number in the range of min - max. Primarily used for testing
 *
 * @param dimension dimension of the created vector
 * @param min minimum number value
 * @param max maximum number value
 * @return
 */
inline std::vector<double> generateRandomVector(int dimension, int min, int max) {

    std::random_device rnd_device;
    // Specify the engine and distribution.
    std::mt19937 mersenne_engine(rnd_device());
    std::uniform_real_distribution<double> dist(min, max);

    auto gen = std::bind(dist, mersenne_engine);
    std::vector<double> vec(dimension);
    generate(begin(vec), end(vec), gen);
    return vec;
}

/**
 * @brief Class for a custom exception for vector operations
 */
class VectorException : public std::runtime_error {
  public:
    /**
     * @brief Construct a new Vector Exception object
     *
     * @param _aMessage
     */
    VectorException(std::string _aMessage) : runtime_error(_aMessage) {}
};

/**
 * @brief Calculates the term frequency of a given term inside a given document
 *
 * @param term the term to calculate the frequency of
 * @param content the content string in which the term appears
 * @return the term frequency
 */
inline float calcTF(const std::string& term, const std::string& content) {

    // TODO: max frequency of any word in the dox instead of the 10 here
    return static_cast<float>((1 + log10(countWordInString(content, term, false))) / (1 + log10(10)));
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
 * @brief Wrapper method for <calcCosineSimilarity>"()" which accepts documents instead of the raw vector
 *
 * @param doc_a
 * @param doc_b
 * @return the cosine similarity
 */
inline float calcCosineSimilarity(const Document& doc_a, const Document& doc_b) {
    // TODO:
    return 0.0; // calcCosineSimilarity(doc_a.getTF_IDF(), doc_b.getTF_IDF())
}

/**
 * @brief Calculates the cosine similarity between #aTF_IDF_a and #aTF_IDF_b
 *
 * @param aTF_IDF_a a tf-idf vector
 * @param aTF_IDF_b a tf-idf vector
 * @return the cosine similarity
 */
inline float calcCosineSimilarity(const std::vector<float>& aTF_IDF_a, const std::vector<float>& aTF_IDF_b) {

    if (aTF_IDF_a.size() != aTF_IDF_b.size()) throw VectorException("Vectors are not of the same size");

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

inline float calcCosineDistance(const Document& doc_a, const Document& doc_b) {
    // TODO
    return 0;
}

/**
 * @brief Calculates the euclidean distance between #doc_a and #doc_b
 *
 * @param doc_a a document
 * @param doc_b a document
 * @return the euclidean distance as double
 */
inline float calcEuclideanDistance(const std::vector<float>& doc_a, const std::vector<float>& doc_b) {

    if (doc_a.size() != doc_b.size()) throw VectorException("Vectors are not of the same size");

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
inline float calcEuclideanDistanceNormalized(std::vector<float> doc_a, std::vector<float> doc_b) {

    if (doc_a.size() != doc_b.size()) throw VectorException("Vectors are not of the same size");

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

    return calcEuclideanDistance(doc_a, doc_b);
}
