//
// Created by Alexander Weiß on 31.03.18.
//

#pragma once

#include <string>
namespace random_projection {

    int dimension(int& sample, float eps = 0.1);
    void create_random_matrix(int rows, int cols, bool JLT, double eps, std::string projection);

} // namespace random_projection