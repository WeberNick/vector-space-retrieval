#include "file_util.hh"

namespace Util
{
    void readIn(const std::string& aPath, const char aDelimiter, string_vvt& aOutput)
    {
        TRACE(std::string("Start reading the file content at '") + aPath + std::string("'"));
        std::ifstream file(aPath, std::ifstream::in);
        std::string line;
        size_t i = 0;
        while (std::getline(file, line)) {
            aOutput.push_back(string_vt());
            Util::splitStringBoost(line, aDelimiter, aOutput[i]);
            ++i;
        }
        file.close();
        TRACE("Finished reading the file content");
    }
}
