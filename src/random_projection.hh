//
// Created by Alexander Weiß on 31.03.18.
//

#pragma once

#include <lib/eigen/Eigen/Dense>
#include <string>
namespace random_projection {

    int dimension(int& sample, float eps = 0.1);
    Eigen::MatrixXd createRandomMatrix(int rows, int cols, bool JLT, double eps = 0.1, std::string projection = "gaussian");
    Eigen::MatrixXd projectMatrix();

} // namespace random_projection