
#include "similarity_measures.hh"
#include "utils.hh"

#include <algorithm>
#include <cmath>
#include <document_manager.hh>
#include <iterator>
#include <sstream>
#include <string>


/**
 * @brief Calculates the term frequency of a given term inside a given document
 *
 * @param term the term to calculate the frequency of
 * @param content the content string in which the term appears
 * @return the term frequency
 */
double similarity_measures::tf(const std::string& term, std::string& content) {

  // TODO: max frequency of any word in the dox instead of the 10 here

  return ((1 + log10(utils::count_word_in_string(content, term, false))) / (1 + log10(10)));
}

/**
 * @brief Calculates the inverted document of a given term inside a given document collection
 *
 * @param term the term to calculate the frequency of
 * @param documentManager the document in which the term appears
 * @return the inverted document frequency
 */
double similarity_measures::idf(const std::string& term, const DocumentManager& documentManager) {}

/**
 * Calculates the tf-idf value
 * @param tf the term frequency
 * @param idf  the inverse document frequency
 * @return the idf value
 */
double similarity_measures::tf_idf(const double& tf, const double& idf) { return tf * idf; }

/**
 * @brief Calculates the cosine similarity between two documents
 *
 * @param doc_a a document
 * @param doc_b a document
 * @return the cosine similarity as double
 */
double similarity_measures::cosine_similarity(const std::vector<double>& doc_a, const std::vector<double>& doc_b) {

  if (doc_a.size() != doc_b.size()) throw VectorException("Vectors are not of the same size");

  double dotProduct = 0;
  double magnitudeDoc_a = 0;
  double magnitudeDoc_b = 0;

  for (int i = 0; i < doc_a.size(); ++i) {
    dotProduct += (doc_a[i] * doc_b[i]);
    magnitudeDoc_a += pow(doc_a[i], 2);
    magnitudeDoc_b += pow(doc_b[i], 2);
  }

  return (dotProduct / (sqrt(magnitudeDoc_a) * sqrt(magnitudeDoc_b)));
}

/**
 * @brief Calculates the euclidean distance between two documents
 *
 * @param doc_a a document
 * @param doc_b a document
 * @return the euclidean distance as double
 */
double similarity_measures::euclidean_distance(const std::vector<double>& doc_a, const std::vector<double>& doc_b) {

  if (doc_a.size() != doc_b.size()) throw VectorException("Vectors are not of the same size");

  double sum = 0;
  for (int i = 0; i < doc_a.size(); ++i) {
    sum += pow((doc_a[i] - doc_b[i]), 2);
  }
  return sqrt(sum);
}

/**
 * @brief Calculates the normalized euclidean distance between two documents
 *
 * @param doc_a a document
 * @param doc_b a document
 * @return the euclidean distance as double
 */
double similarity_measures::euclidean_distance_normalized(std::vector<double> doc_a, std::vector<double> doc_b) {

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

  return similarity_measures::euclidean_distance(doc_a, doc_b);
}
