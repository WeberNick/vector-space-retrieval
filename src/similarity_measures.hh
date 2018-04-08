// Created by Alexander Weiß on 30.03.18.
//

#pragma once

#include "document.hh"
#include "document_manager.hh"
#include <string>

/**
 * @brief Namespace for similarity measure methods
 */
namespace SimilarityMeasures {

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

unsigned long count_word_in_string(std::string const& str, std::string const& word, bool case_insensitive);
double tf(const std::string& term, std::string& content);
double idf(const std::string& term, const DocumentManager& documentManager);
double tf_idf(const double& tf, const double& idf);
double cosine_similarity(const std::vector<double>& doc_a, const std::vector<double>& doc_b);
float calcCosineDist(const Document& aDoc, const Document& aOtherDoc);
double euclidean_distance(const std::vector<double>& doc_a, const std::vector<double>& doc_b);
double euclidean_distance_normalized(std::vector<double> doc_a, std::vector<double> doc_b);

} // namespace similarity_measures
