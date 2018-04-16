/**
 *	@file 	trace.hh
 *	@author	Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *	@brief  Header file implementing tracing functionality
 *	@bugs 	Currently no bugs known
 *	@todos	Write DESCRIPTION
 *
 *	@section DESCRIPTION
 *	TODO
 */
#pragma once

#include <string>

/**
 * @brief the different TraceLevels severe, warning and info
 * 
 */
enum struct TraceLevel {
    severe = 0,
    warning = 1,
    info = 2
}

/**
 * @brief the Trace namespace
 * 
 */
namespace Trace {
    bool _isTraceOn;
    void print(TraceLevel callLevel, TraceLevel msgLevel, const std::string& aTerm) {
        if (_isTraceOn && msgLevel <= callLevel) {
            std::cout << aTerm << std::endl;
        }
    }
} // namespace Trace