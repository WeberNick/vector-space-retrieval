/*
 * @file    file_util.hh
 * @author 	Nick Weber
 * @date    Mai 19, 2018
 * @brief 	Some file utils 
 * 
 */
#pragma once

#include "types.hh"
#include "exception.hh"
#include "trace.hh"
#include "string_util.hh"

#include <string>
#include <vector>

namespace Util {
    /**
     * @brief Reads in the file content line by line and splits each line at the specified delimiter
     *
     *
     */
    void readIn(const std::string& aPath, const char aDelimiter, string_vvt& aOutput);
}