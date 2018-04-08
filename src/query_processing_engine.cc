#include "query_processing_engine.hh"

std::string QueryProcessingEngine::search(const std::string& string) { return Utility::IR::stemPorter(string); }
