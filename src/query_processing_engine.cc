#include "query_processing_engine.hh"
#include <iostream>
#include <lib/oleanderStemmingLibrary/stemming/english_stem.h>
#include <sstream>

std::wstring s2ws(const std::string& s) {
    std::wstring ws;
    ws.assign(s.begin(), s.end());
    return ws;
}

std::string QueryProcessingEngine::search(const std::string& string) {

    std::istringstream iss(string);
    std::string word;

    stemming::english_stem<> StemEnglish;

    while (iss >> word) {

        std::wstring wordToStem = s2ws(word);
        StemEnglish(wordToStem);
        std::wcout << wordToStem << std::endl;
    }

    return string;
}
