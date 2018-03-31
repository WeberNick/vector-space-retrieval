//
// Created by Alexander Weiß on 31.03.18.
//

#include "random_projection.h"
#include "cmath"
#include <vector>

/**
 * @brief  Find the minimum dimension required to project the data from high dimensional space to low dimensional space
 *
 * @param sample number of samples
 * @param eps error tolerance level
 * @return minimum amount of dimensions
 */
int random_projection::dimension(int& sample, float eps) {
    if (eps > 1.0 | eps <= 0.0) {
        throw("The JL bound for epsilon is [0.0,1.0]");
    } else if (sample <= 0.01) {
        throw("Sample size must be grater than zero");
    } else {
        double denominator = (pow(eps, 2) / 2 - (pow(eps, 3) / 3));
        return floor((4 * log(sample) / denominator));
    }
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
void random_projection::create_random_matrix(int rows, int cols, bool JLT, double eps = 0.1, std::string projection = "gaussian") {}
