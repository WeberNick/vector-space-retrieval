//
// Created by Alexander Weiß on 31.03.18.
//

#include "random_projection.hh"
#include "cmath"
#include "utility.hh"
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
Eigen::MatrixXf random_projection::createRandomMatrix(int rows, int cols, bool JLT, double eps, std::string projection) {

    if (rows == 0) throw("Number of rows has to be greater than 0");
    if (cols == 0) throw("Number of columns has to be greater than 0");

    if (JLT) { cols = dimension(cols, eps); }

    std::vector<double> randoms;

    for (int i = 0; i < cols * rows; ++i) {
        double rand = (rand_normal(0, (1 / sqrt(cols))));
        randoms.push_back(rand);
    }

    Eigen::MatrixXf m(rows, cols);
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
Eigen::MatrixXf random_projection::projectMatrix() {

    int nrow = 5;
    int ncol = 103260;
    int j = ncol;

    Eigen::MatrixXf randomMatrix = (Eigen::MatrixXf::Random(nrow, ncol) + Eigen::MatrixXf::Ones(nrow, ncol)) * 5;
    std::cout << "Random matrix generated" << std::endl;

    Eigen::VectorXf v1 = randomMatrix.row(0);
    std::vector<float> doc_a(&v1[0], v1.data() + v1.cols() * v1.rows());

    Eigen::VectorXf v2 = randomMatrix.row(1);
    std::vector<float> doc_b(&v2[0], v2.data() + v2.cols() * v2.rows());

    std::cout << "Cosine similarity between doc_a and doc_b before reduction: " << Utility::SimilarityMeasures::calcCosSim(doc_a, doc_b) << std::endl;

    int k = dimension(j, 0.5);
    std::cout << "New dimension: " << k << std::endl;
    // eps = 0.1, projection = "gaussian"

    std::cout << "Create projection matrix" << std::endl;
    Eigen::MatrixXf projectionMatrix = createRandomMatrix(j, k, false);
    std::cout << "Projection matrix generated" << std::endl;

    std::cout << "Project matrix into a lower dimensional space" << std::endl;
    Eigen::MatrixXf resultMatrix = randomMatrix * projectionMatrix;

    std::cout << "Here is the result " << std::endl;
    std::cout << "Original cols: " << randomMatrix.cols() << std::endl;
    std::cout << "Result cols: " << resultMatrix.cols() << std::endl;

    Eigen::VectorXf v4 = resultMatrix.row(0);
    std::vector<float> doc_c(&v4[0], v4.data() + v4.cols() * v4.rows());

    Eigen::VectorXf v5 = resultMatrix.row(1);
    std::vector<float> doc_d(&v5[0], v5.data() + v5.cols() * v5.rows());

    std::cout << "Cosine similarity between doc_a and doc_b before reduction: " << Utility::SimilarityMeasures::calcCosSim(doc_c, doc_d) << std::endl;

    return Eigen::MatrixXf();
}
