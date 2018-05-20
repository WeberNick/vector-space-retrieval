#include "string_util.hh"
#include "measure.hh"

namespace Util {
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

    std::string& toLower(std::string& aString) {
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
        for (auto& str : aStrings) {
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
        splitStringBoost(strCased, ' ', content);

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

    std::string& ltrim(std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
        return s;
    }

    std::string& rtrim(std::string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
        return s;
    }

    std::string& trim(std::string& s) { return ltrim(rtrim(s)); }

    std::string ltrim_copy(const std::string& s) {
        std::string cp = s;
        return ltrim(cp);
    }

    std::string rtrim_copy(const std::string& s) {
        std::string cp = s;
        return rtrim(cp);
    }

    std::string trim_copy(const std::string& s) {
        std::string cp = s;
        return trim(cp);
    }

    void removeStopword(string_vt& str, const str_set& stopwordList) {
       str.erase(std::remove_if(
            str.begin(),
            str.end(),
            [stopwordList](const std::string& s){ return stopwordList.find(s) != stopwordList.end(); }),
            str.end());
    }

    string_vt preprocess(std::string& aContent, const str_set& aStopwordsList) {

        std::cout << "toLower" << std::endl;
        Measure m1;
        m1.start();
        Util::toLower(aContent);
        m1.stop();
        std::cout << "toLower took " << m1.mTotalTime() << std::endl;

        std::cout << "remove_copy_if" << std::endl;
        Measure m3;
        m3.start();
        std::string temp;
        std::remove_copy_if(aContent.begin(), aContent.end(),
                            std::back_inserter(temp), // Store output
                            std::ptr_fun<int, int>(&std::ispunct));
        aContent = temp;
        m3.stop();
        std::cout << "remove_copy_if took " << m3.mTotalTime() << std::endl;

        std::cout << "trim" << std::endl;
        Measure m4;
        m4.start();
        Util::trim(aContent); // Trim whitespaces at front and end
        m4.stop();
        std::cout << "trim took " << m4.mTotalTime() << std::endl;

        string_vt lProcessed;
        
        
        std::cout << "splitStringBoost" << std::endl;
        Measure m5;
        m5.start();
        Util::splitStringBoost(aContent, ' ', lProcessed); // Split string by whitespaces
        m5.stop();
        std::cout << "splitStringBoost took " << m5.mTotalTime() << std::endl;

        std::cout << "removeStopword" << std::endl;
        Measure m2;
        m2.start();
        Util::removeStopword(lProcessed, aStopwordsList); // Remove stopwords
        m2.stop();
        std::cout << "removeStopword took " << m2.mTotalTime() << std::endl;

        std::cout << "removeEmptyStringsFromVec" << std::endl;
        Measure m6;
        m6.start();
        Util::removeEmptyStringsFromVec(lProcessed);       // Remove eventually empty strings from the query term vector
        m6.stop();
        std::cout << "removeEmptyStringsFromVec took " << m6.mTotalTime() << std::endl;


        std::cout << "stemPorter" << std::endl;
        Measure m7;
        m7.start();
        string_vt preprocessed_content;
        for (auto& elem : lProcessed) { // Preprocess query
            std::string preprocess = Util::stemPorter(elem);
            preprocessed_content.push_back(preprocess);
        }
        m7.stop();
        std::cout << "stemPorter took " << m7.mTotalTime() << std::endl;


        return preprocessed_content;
    }
} // namespace Util
