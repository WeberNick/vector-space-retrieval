#include "ir_util.hh"

namespace Util
{
        float calcTf(const float term, const float max) { return static_cast<float>((1 + log10(term)) / (1 + log10(max))); }

        float calcIdf(const float N, const float docs) { return static_cast<float>(log10(N / docs)); }

        float calcTfIdf(const float tf, const float idf) { return tf * idf; }


        //float calcTf(const std::string& term, const std::string& content) {
            //return static_cast<float>((1 + log10(Util::countWordInString(content, term, false))) /
                                      //(1 + log10(Util::getMaxWordFrequency(content))));
        //}

        float calcTf(const std::string& term, const std::vector<std::string>& content) {
            return static_cast<float>((1 + log10(Util::countWordInString(content, term, false))) /
                                      (1 + log10(Util::getMaxWordFrequency(content))));
        }

        std::string stemPorter(const std::string& sentence) {
            std::istringstream iss(sentence);
            std::ostringstream os;
            std::string word;

            stemming::english_stem<> StemEnglish;
            while (iss >> word) {
                std::wstring wordToStem = Util::string2wstring(word);
                StemEnglish(wordToStem);
                os << Util::wstring2string(wordToStem) << " ";
            }
            std::string stemmed = os.str();
            Util::trim(stemmed);
            return stemmed;
        }

        std::map<size_t, PostingList> calculateTiers(const size_t aNumTiers, const PostingList& aPostingList) {
            const sizet_float_mt& aPosting = aPostingList.getPosting();
            const float idf = aPostingList.getIdf();
            std::map<size_t, PostingList> outputMap;

            std::vector<std::pair<size_t, float>> vec; // vector of <id, tf> pairs, will be sorted descending by tf
            for (auto it = aPosting.begin(); it != aPosting.end(); ++it) {
                vec.push_back(*it);
            }
            std::sort(vec.begin(), vec.end(), [](auto& a, auto& b) { return a.second > b.second; }); // desc

            size_t size = aPosting.size();
            uint boundary = std::floor((double)size / aNumTiers);
            for (size_t tier = 0; tier < aNumTiers; ++tier) {
                sizet_float_mt posting;
                if (size < aNumTiers && !vec.empty()) {
                    posting.insert(Util::pop_front(vec));
                } else {
                    boundary = (tier == (aNumTiers - 1)) ? vec.size() : boundary;
                    for (size_t i = 0; i < boundary; ++i) {
                        posting.insert(Util::pop_front(vec));
                    }
                }
                PostingList postinglist(idf, posting);
                outputMap.insert(std::make_pair(tier, postinglist));
            }
            return outputMap;
        }

        void andPostingLists(sizet_vt& first, sizet_vt& second, sizet_vt& out) {
            std::sort(first.begin(), first.end(), [](const size_t a, const size_t b) { return a < b; });
            std::sort(second.begin(), second.end(), [](const size_t a, const size_t b) { return a < b; });
            auto ione = first.begin();
            auto itwo = second.begin();
            out.clear();
            while (ione != first.end() && itwo != second.end()) {
                if (*ione == *itwo) {
                    out.push_back(*ione);
                    ++ione;
                    ++itwo;
                } else if (*ione < *itwo) {
                    ++ione;
                } else
                    ++itwo;
            }
        }

        void andPostingLists(std::vector<sizet_vt>& vecs, sizet_vt& out) {
            out.clear();
            if (vecs.size() > 1) {
                std::sort(vecs.begin(), vecs.end(), [](const sizet_vt& a, const sizet_vt& b) { return a.size() < b.size(); }); // asc
                andPostingLists(vecs.at(0), vecs.at(1), out);
                if (vecs.size() == 2) return;
                for (size_t i = 2; i < vecs.size(); ++i) {
                    sizet_vt out_copy(out);
                    andPostingLists(out_copy, vecs.at(i), out);
                }
            } else {
                out = vecs.at(0);
            }
        }

        void orPostingLists(std::vector<sizet_vt>& vecs, sizet_vt& out) {
            out.clear();
            if (vecs.size() > 1) {
                for (auto& vec : vecs) {
                    for (auto& elem : vec) {
                        out.push_back(elem);
                    }
                }
            } else {
                out = vecs.at(0);
            }
            std::sort(out.begin(), out.end(), [](const size_t a, const size_t b) { return a < b; }); // asc
            out.erase(std::unique(out.begin(), out.end()), out.end());
        }
}
