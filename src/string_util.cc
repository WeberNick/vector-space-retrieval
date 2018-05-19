#include "string_util.hh"

namespace Util
{
        std::string string_vt_2_str(const string_vt& stringVector) {
            std::string str;

            for (size_t j = 0; j < stringVector.size() - 1; ++j) {
                str += stringVector[j] + " ";
            }
            str += stringVector[stringVector.size() - 1];
            return str;
        }

        void splitStringBoost(const std::string& aString, const char aDelimiter, string_vt& aOutputVector) {
            boost::split(aOutputVector, aString, boost::is_any_of(std::string(1, aDelimiter)));
        }

        bool endsWith(const std::string& aString, const std::string& aSuffix) { return boost::algorithm::ends_with(aString, aSuffix); }

        std::string toLower(const std::string& aString) {
            std::string data = aString;
            std::transform(data.begin(), data.end(), data.begin(), ::tolower);
            return data;
        }

        std::string& toLower(std::string& aString)
        {
            std::transform(aString.begin(), aString.end(), aString.begin(), ::tolower);
            return aString;
        }


        string_vt toLower(const string_vt& aString) {
            string_vt data;
            data.reserve(aString.size());
            std::transform(aString.begin(), aString.end(), std::back_inserter(data), [](const std::string& in) {
                std::string out;
                out.reserve(in.size());
                std::transform(in.begin(), in.end(), std::back_inserter(out), ::tolower);
                return out;
            });
            return data;
        }

        string_vt& toLower(string_vt& aStrings) {
            for(auto& str : aStrings)
            {
                toLower(str);
            }
            return aStrings;
        }

        std::wstring string2wstring(const std::string& s) {
            std::wstring ws;
            ws.assign(s.begin(), s.end());
            return ws;
        }

        std::string wstring2string(const std::wstring& ws) {
            std::string s;
            s.assign(ws.begin(), ws.end());
            return s;
        }

        size_t countWordInString(const std::string& str, const std::string word, bool case_insensitive) {
            std::string wordCased = case_insensitive ? toLower(word) : word;
            std::string strCased = case_insensitive ? toLower(str) : str;

            string_vt content;
            splitString(strCased, ' ', content);

            size_t count = 0;
            for (size_t i = 0; i < content.size(); ++i) {
                if (content[i] == wordCased) ++count;
            }
            return count;
        }

        size_t countWordInString(const string_vt& str, const std::string& word, bool case_insensitive) {

            std::string word_2 = case_insensitive ? toLower(word) : word;
            string_vt str_2 = case_insensitive ? toLower(str) : str;

            size_t count = 0;
            for (const auto& i : str) {
                if (i == word) ++count;
            }
            return count;
        }

        int getMaxWordFrequency(const std::string& str) {
            if (str == "") return 0;
            std::istringstream input(str);
            std::map<std::string, int> count;
            std::string word;
            decltype(count)::const_iterator most_common;
            while (input >> word) {
                auto iterator = count.emplace(word, 0).first;
                ++iterator->second;
                if (count.size() == 1 || iterator->second > most_common->second) most_common = iterator;
            }
            return most_common->second;
        }

        int getMaxWordFrequency(string_vt vec) {
            if (vec.size() == 0)
                return 0;
            else if (vec.size() == 1)
                return getMaxWordFrequency(vec.at(0));
            else {
                std::map<std::string, int> count;
                std::string word;

                for (const auto& i : vec) {
                    ++count[i];
                }
                int maxn = max_element(count.begin(), count.end(),
                                       [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) { return a.second < b.second; })
                               ->second;
                return maxn;
            }
        }

        void removeEmptyStringsFromVec(string_vt& vec) {
            vec.erase(std::remove_if(vec.begin(), vec.end(), [](const std::string& s) { return s.empty(); }), vec.end());
        }
}