//
// Created by Alexander Weiß on 31.03.18.
//

#include "random_projection.hh"
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
        return static_cast<int>(floor((4 * log(sample) / denominator)));
    }
}

// TODO: RANDOM PROJECT IS NOT WORKING
/**
 * @brief
 *
 * @param rows
 * @param cols
 * @param JLT
 * @param eps
 * @param projection
 */
Eigen::MatrixXd random_projection::createRandomMatrix(int rows, int cols, bool JLT, double eps, std::string projection) {

    if (rows == 0) throw("Number of rows has to be greater than 0");
    if (cols == 0) throw("Number of columns has to be greater than 0");

    if (JLT) { cols = dimension(cols, eps); }

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

    return m;
}
Eigen::MatrixXd random_projection::projectMatrix() {

    int rows = 5;
    int cols = 1000;
    int j = cols;

    Eigen::MatrixXd randomMatrix = Eigen::MatrixXd::Random(rows, cols);
    std::cout << "Random matrix" << std::endl;
    std::cout << randomMatrix << std::endl;

    int k = dimension(j);
    std::cout << "New dimension: " << k << std::endl;
    // eps = 0.1, projection = "gaussian"

    Eigen::MatrixXd projectionMatrix = createRandomMatrix(j, k, false);
    std::cout << "Random matrix generated" << std::endl;
    std::cout << projectionMatrix << std::endl;

    std::cout << "Project matrix into a lower dimensional space" << std::endl;
    Eigen::MatrixXcd resultMatrix = randomMatrix * projectionMatrix;

    std::cout << "Here is the result " << std::endl;
    std::cout << resultMatrix << std::endl;

    std::cout << "Start cols: " << randomMatrix.cols() << std::endl;
    std::cout << "Result cols: " << resultMatrix.cols() << std::endl;

    /*Eigen::MatrixXd a(2,3);
    a << 3, 2, 1,
        1, 0, 2;
    std::cout << a << std::endl;
    std::cout << "======================"<< std::endl;


    Eigen::MatrixXd b(3,2);
    b << 1, 2,
         0, 1,
         4, 0;

    Eigen::MatrixXd c = a * b;


    std::cout << b << std::endl;
  std::cout << "======================" << std::endl;
    std::cout << c << std::endl;*/

    return Eigen::MatrixXd();
}
