//
// Created by Alexander Weiß on 30.03.18.
//
#pragma once

/**
 * @brief Namespace for utility functions
 *
 */
#include "string"
#include <vector>

namespace utils {

  std::string to_lower(std::string string);

  unsigned long count_word_in_string(std::string const& str, std::string const& word, bool const case_insensitive);

  std::vector<double> generate_random_vector(int dimension, int min, int max);

} // namespace utils