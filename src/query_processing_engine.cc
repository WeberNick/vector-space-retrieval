#include "query_processing_engine.hh"
#include "utility.hh"
#include <iostream>

#include <sstream>

std::string QueryProcessingEngine::search(const std::string& string) { return IR::stemPorter(string); }
