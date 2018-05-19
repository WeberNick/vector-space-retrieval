/*********************************************************************
 * @file    file_util.hh
 * @author 	Nick Weber
 * @date    Mai 19, 2018
 * @brief 	
 * @bugs 	TBD
 * @todos 	TBD
 * 
 * @section	DESCRIPTION
 * TBD
 * 
 * @section USE
 * TBD
 ********************************************************************/
#pragma once

#include "types.hh"
#include "exception.hh"
#include "trace.hh"
#include "string_util.hh"
#include "document.hh"

#include <srtring>
#include <vector>

namespace Util
{
    /**
     * @brief Reads in the file content line by line and splits each line at the specified delimiter
     *
     *
     */
    void readIn(const std::string& aPath, const char aDelimiter, string_vvt& aOutput);

    void readInAndCreateDocs(const std::string& aPath, const char aDelimiter, doc_mt& aDocMap);

}
