#pragma once

#include "../document.hh"
#include <math.h>
#include <stdexcept>

namespace similarity_measures {

/**
 * Exception class for vector methods
 */
class VectorException : public std::runtime_error {
 public:
  VectorException(std::string _aMessage) : runtime_error(_aMessage) {}
};

/**
 * Calculates the term frequency of a given term inside a given document
 * @param term the term to calculate the frequency of
 * @param doc the document in which the term appears
 * @return the term frequency
 */
double tf(const std::string &term, const Document &doc) {}

double idf() {}

/**
 * Calculates the tf-idf value
 * @param tf the term frequency
 * @param idf  the inverse document frequency
 * @return the idf value
 */
double tf_idf(const double &tf, const double &idf) {
  return tf*idf;
}

/**
 * Calculates the cosine similarity between two documents
 * @param doc_a a document
 * @param doc_b a document
 * @return the cosine similarity as double
 */
double cosine_similarity(const std::vector<double> &doc_a, const std::vector<double> &doc_b) {

  if (doc_a.size()!=doc_b.size()) throw VectorException("Vectors are not of the same size");

  double dotProduct;
  double magnitudeDoc_a;
  double magnitudeDoc_b;

  for (int i = 0; i < doc_a.size(); ++i) {
    dotProduct += (doc_a[i]*doc_b[i]);
    magnitudeDoc_a += pow(doc_a[i], 2);
    magnitudeDoc_b += pow(doc_b[i], 2);
  }

  return (dotProduct/(sqrt(magnitudeDoc_a)*sqrt(magnitudeDoc_b)));

}

/**
 * Calculates the euclidean distance between two documents
 * @param doc_a a document
 * @param doc_b a document
 * @return the euclidean distance as double
 */
double euclidean_distance(const std::vector<double> &doc_a, const std::vector<double> &doc_b) {

  if (doc_a.size()!=doc_b.size()) throw VectorException("Vectors are not of the same size");

  double sum;
  for (int i = 0; i < doc_a.size(); ++i) {
    sum += pow((doc_a[i] - doc_b[i]), 2);
  }
  return sqrt(sum);
}

/**
 * Calculates the normalized euclidean distance between two documents
 * @param doc_a a document
 * @param doc_b a document
 * @return the euclidean distance as double
 */
double euclidean_distance_normalized(std::vector<double> doc_a, std::vector<double> doc_b) {

  if (doc_a.size()!=doc_b.size()) throw VectorException("Vectors are not of the same size");

  double magnitudeDoc_a;
  double magnitudeDoc_b;

  for (int i = 0; i < doc_a.size(); ++i) {
    magnitudeDoc_a += pow(doc_a[i], 2);
    magnitudeDoc_b += pow(doc_b[i], 2);
  }

  magnitudeDoc_a = sqrt(magnitudeDoc_a);
  magnitudeDoc_b = sqrt(magnitudeDoc_b);

  for (int j = 0; j < doc_a.size(); ++j) {
    doc_a[j] = doc_a[j]/magnitudeDoc_a;
    doc_b[j] = doc_b[j]/magnitudeDoc_b;
  }

  return euclidean_distance(doc_a, doc_b);
}


/**
 * Projects a vector down to an @param dimension dimensional space
 * @param tf_vec a vector
 * @param dimension a document
 * @return a new @param dimension dimensional space
 */
std::vector<double> random_projection(const std::vector<double> &tf_vec, int dimension) {

}

} // namespace similarity_measures
