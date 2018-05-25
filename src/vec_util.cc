#include "vec_util.hh"

namespace Util {

    sizet_vt difference(const sizet_vt& v1, const sizet_vt& v2) {
        sizet_vt lDifference;
        sizet_vt lv1 = v1;
        sizet_vt lv2 = v2;
        std::sort(lv1.begin(), lv1.end());
        std::sort(lv2.begin(), lv2.end());
        std::set_difference(lv1.begin(), lv1.end(), lv2.begin(), lv2.end(), std::inserter(lDifference, lDifference.begin()));
        return lDifference;
    }

    sizet_vt intersection(const sizet_vt& v1, const sizet_vt& v2) {
        sizet_vt lIntersection;
        sizet_vt lv1 = v1;
        sizet_vt lv2 = v2;
        std::sort(lv1.begin(), lv1.end());
        std::sort(lv2.begin(), lv2.end());
        std::set_intersection(lv1.begin(), lv1.end(), lv2.begin(), lv2.end(), std::inserter(lIntersection, lIntersection.begin()));
        return lIntersection;
    }

    size_t numberOfDifferences(const sizet_vt& v1, const sizet_vt& v2) {
        return difference(v1, v2).size();
    }

    size_t numberOfIntersections(const sizet_vt& v1, const sizet_vt& v2) {
        return intersection(v1, v2).size();
    }

    float_vt generateRandomVector(size_t dimension, int min, int max, uint& seed) {
        std::random_device rnd_device;
        // increment the reference to the original seed, since if using the same seed over and over again the random projections are always the same 
        std::mt19937 mersenne_engine((seed++)); 
        std::uniform_real_distribution<float> dist(min, max);

        auto gen = std::bind(dist, std::ref(mersenne_engine));
        float_vt vec(dimension);
        generate(begin(vec), end(vec), gen);
        return vec;
    }

    float_vt combineVectors(const float_vt& a, const float_vt& b) {
        float_vt result(a.size() + b.size());
        result.insert( result.end(), a.begin(), a.end() );
        result.insert( result.end(), b.begin(), b.end() );
        return result;
    }
}
