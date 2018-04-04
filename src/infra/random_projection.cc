//
// Created by Alexander Weiß on 31.03.18.
//

#include "random_projection.h"
#include "cmath"
#include <Eigen/Dense>
#include <iostream>
#include <vector>

double rand_normal(double mean, double stddev) { // Box muller method
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
void random_projection::create_random_matrix(int rows, int cols, bool JLT, double eps = 0.1, std::string projection = "gaussian") {

    std::vector<double> randoms;

    for (int i = 0; i < cols * rows; ++i) {
        double rand = (rand_normal(0, (1 / sqrt(cols))));
        randoms.push_back(rand);
    }

    Eigen::MatrixXd m(rows, cols);
    int k = 0;
    for (int j = 0; j < rows; ++j) {
        for (int i = 0; i < cols; ++i) {
            double rand = randoms[k];
            m(j, i) = randoms[k];
            k++;
        }
    }

    std::cout << m << std::endl;
    std::cout << m(99, 499) << std::endl;
}
