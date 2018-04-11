//
// Created by Alexander Weiß on 31.03.18.
//

#pragma once

#include <Eigen/Dense>
#include <string>

namespace random_projection {

    int dimension(int& sample, float eps = 0.1);
    Eigen::MatrixXf createRandomMatrix(int rows, int cols, bool JLT, double eps = 0.1, std::string projection = "gaussian");
    Eigen::MatrixXf projectMatrix();

} // namespace random_projection