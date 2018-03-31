//
// Created by Alexander Weiß on 31.03.18.
//

#include <vector>
#include "random_projection.h"


/**
 * @brief  Find the minimum dimension required to project the data from high dimensional space to low dimensional space
 *
 * @param sample number of samples
 * @param eps error tolerance level
 * @return minimum amount of dimensions
 */
int random_projection::dimension(int &sample, float eps = 0.1) {
  return 0;
}


/**
 * @brief Generates a random matrix based on different projection algorithms
 *
 * @param rows number of rows
 * @param cols number of columns
 * @param JLT
 * @param eps error tolerance level with default value 0.1
 * @param projection projection function with default value "gaussian"
 */
void random_projection::create_random_matrix(int rows, int cols, bool JLT, double eps = 0.1, std::string projection = "gaussian") {

}


š