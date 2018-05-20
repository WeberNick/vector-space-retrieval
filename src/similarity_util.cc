#include "similarity_util.hh"

namespace Util
{
        float calcCosSim(const float_vt& aTfIdf_a, const float_vt& aTfIdf_b) {
            if (aTfIdf_a.size() != aTfIdf_b.size()) {
                const std::string traceMsg = "Vectors do not have the same size";
                TRACE(traceMsg);
                throw VectorException(FLF, traceMsg);
            }

            if (vectorLength(aTfIdf_a) == 0 || vectorLength(aTfIdf_b) == 0){
                return 0;
            }

            double dotProduct = 0;
            for (size_t i = 0; i < aTfIdf_a.size(); ++i) {
                dotProduct += (aTfIdf_a[i] * aTfIdf_b[i]);
            }

            return static_cast<float>(dotProduct / (vectorLength(aTfIdf_a) * vectorLength(aTfIdf_b)));  
        }

        float calcCosDist(const float_vt& aTF_IDF_a, const float_vt& aTF_IDF_b) { return 1 - calcCosSim(aTF_IDF_a, aTF_IDF_b); }

        float calcCosSim(const Document& doc_a, const Document& doc_b) { return calcCosSim(doc_a.getTfIdfVector(), doc_b.getTfIdfVector()); }

        float calcCosDist(const Document& doc_a, const Document& doc_b) { return calcCosDist(doc_a.getTfIdfVector(), doc_b.getTfIdfVector()); }

        float calcAngularSimilarity(const float_vt& aTfIdf_a, const float_vt& aTfIdf_b) {

            if (aTfIdf_a.size() != aTfIdf_a.size()) 
            {
                const std::string traceMsg = "Vectors do not have the same size";
                TRACE(traceMsg);
                throw VectorException(FLF, traceMsg);
            }

            float cosine = calcCosSim(aTfIdf_a, aTfIdf_b);
            float theta = acosf(cosine);

            return static_cast<float>(1 - (theta / M_PI));
        }

        float calcAngularSimilarity(const Document& doc_a, const Document& doc_b) {
            return calcAngularSimilarity(doc_a.getTfIdfVector(), doc_b.getTfIdfVector());
        }

        unsigned int calcHammingDist(std::vector<bool>& vec_a, std::vector<bool>& vec_b) {

            if (vec_a.size() != vec_a.size()){
                const std::string traceMsg = "Vectors do not have the same size";
                TRACE(traceMsg);
                throw VectorException(FLF, traceMsg);
            }

            unsigned int dist = 0;
            for (size_t i = 0; i < vec_a.size(); ++i) {
                if (vec_a[i] != vec_b[i]) { dist++; }
            }
            return dist;
        }

        unsigned int calcHammingDist(const boost::dynamic_bitset<>& vec_a, const boost::dynamic_bitset<>& vec_b) {
            return static_cast<unsigned int>((vec_a ^ vec_b).count());
        }

        unsigned int calcHammingSim(boost::dynamic_bitset<>& vec_a, boost::dynamic_bitset<>& vec_b) {
            return static_cast<unsigned int>(vec_a.size() - calcHammingDist(vec_a, vec_b));
        }

        float calcAngSimHamming(std::vector<bool>& vec_a, std::vector<bool>& vec_b) {
            int hamming = calcHammingDist(vec_a, vec_b);

            double theta = acosf(cos(((hamming / vec_a.size()) * M_PI)));

            return static_cast<float>(1 - (theta / M_PI));
        }

        float calcEuclDist(const float_vt& doc_a, const float_vt& doc_b) {
            if (doc_a.size() != doc_b.size()) {
                const std::string traceMsg = "Vectors do not have the same size";
                TRACE(traceMsg);
                throw VectorException(FLF, traceMsg);
            }

            double sum = 0;
            for (size_t i = 0; i < doc_a.size(); ++i) {
                sum += pow((doc_a[i] - doc_b[i]), 2);
            }
            return static_cast<float>(sqrt(sum));
        }

        float calcEuclDistNormalized(float_vt& doc_a, float_vt& doc_b) {
            if (doc_a.size() != doc_b.size()) {
                const std::string traceMsg = "Vectors do not have the same size";
                TRACE(traceMsg);
                throw VectorException(FLF, traceMsg);
            }

            double len_a = vectorLength(doc_a);
            double len_b = vectorLength(doc_b);
            for (size_t j = 0; j < doc_a.size(); ++j) {
                doc_a[j] = doc_a[j] / len_a;
                doc_b[j] = doc_b[j] / len_b;
            }
            return calcEuclDist(doc_a, doc_b);
        }

}
