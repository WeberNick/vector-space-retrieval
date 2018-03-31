//
// Created by Alexander Weiß on 30.03.18.
//

#include <string>
#include <algorithm>
#include "utils.hh"


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

