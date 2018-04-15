#pragma once

#include <string>

enum struct TraceLevel {
    severe = 0,
    warning = 1,
    info = 2
}

namespace Trace {
    bool _isTraceOn;
    void print(TraceLevel callLevel, TraceLevel msgLevel, const std::string& aTerm) {
        if (_isTraceOn && msgLevel <= callLevel) {
            std::cout << aTerm << std::endl;
        }
    }
}