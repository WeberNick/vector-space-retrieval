//
// Created by Alexander Weiß on 30.03.18.
//

//
// Created by Alexander Weiß on 30.03.18.
//

#include "util/utils.hh"
#include <algorithm>
#include <functional>
#include <random>
#include <string>
#include <sstream>
#include <iterator>

/**
 * @brief Lower case a given string
 *
 * @param string string to lower case
 * @return lowercased string
 */
std::string utils::to_lower(std::string string) {
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

unsigned long utils::count_word_in_string(std::string const& str, std::string const& word, bool const case_insensitive) {
  if (case_insensitive) {

    std::string word_lower = utils::to_lower(word);
    std::string string_lower = utils::to_lower(str);
    std::istringstream ss(string_lower);

    return static_cast<unsigned long>(std::count_if(std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>(),
                                                    [word_lower](const std::string& s) { return s == word_lower; }));
  } else {

    std::istringstream ss(str);

    return static_cast<unsigned long>(
        std::count_if(std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>(), [word](const std::string& s) { return s == word; }));
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
std::vector<double> utils::generate_random_vector(int dimension, int min, int max) {

  std::random_device rnd_device;
  // Specify the engine and distribution.
  std::mt19937 mersenne_engine(rnd_device());
  std::uniform_real_distribution<double> dist(min, max);

  auto gen = std::bind(dist, mersenne_engine);
  std::vector<double> vec(dimension);
  generate(begin(vec), end(vec), gen);
  return vec;
}
