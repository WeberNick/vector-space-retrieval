#include "vec_util.hh"

namespace Util {

    sizet_vt difference(const sizet_vt& v1, const sizet_vt& v2)
    {
        sizet_vt lDifference;
        sizet_vt lv1 = v1;
        sizet_vt lv2 = v2;
        std::sort(lv1.begin(), lv1.end());
        std::sort(lv2.begin(), lv2.end());
        std::set_difference(lv1.begin(), lv1.end(), lv2.begin(), lv2.end(), std::inserter(lDifference, lDifference.begin()));
        return lDifference;
    }

    sizet_vt intersection(const sizet_vt& v1, const sizet_vt& v2)
    {
        sizet_vt lIntersection;
        sizet_vt lv1 = v1;
        sizet_vt lv2 = v2;
        std::sort(lv1.begin(), lv1.end());
        std::sort(lv2.begin(), lv2.end());
        std::set_intersection(lv1.begin(), lv1.end(), lv2.begin(), lv2.end(), std::inserter(lIntersection, lIntersection.begin()));
        return lIntersection;
    }

    size_t numberOfDifferences(const sizet_vt& v1, const sizet_vt& v2) 
    {
        return difference(v1, v2).size();
    }

    size_t numberOfIntersections(const sizet_vt& v1, const sizet_vt& v2) 
    {
        return intersection(v1, v2).size();
    }

    double rand_normal(double mean, double stddev) {
        static double n2 = 0.0;
        static int n2_cached = 0;
        if (!n2_cached) {
            double x, y, r;
            do {
                x = 2.0 * rand() / RAND_MAX - 1;
                y = 2.0 * rand() / RAND_MAX - 1;

                r = x * x + y * y;
            } while (r == 0.0 || r > 1.0);
            {
                double d = sqrt(-2.0 * log(r) / r);
                double n1 = x * d;
                n2 = y * d;
                double result = n1 * stddev + mean;
                n2_cached = 1;
                return result;
            }
        } else {
            n2_cached = 0;
            return n2 * stddev + mean;
        }
    }

    float_vt generateRandomVector(size_t dimension, int min, int max) {

        std::random_device rnd_device;
        // Specify the engine and distribution.
        std::mt19937 mersenne_engine(rnd_device());
        std::uniform_int_distribution<int> dist(min, max);

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

    float_vt generateRandomVectorN(size_t dimension) {
        float_vt result(dimension);
        for (size_t i = 0; i < dimension; ++i) {
            result[i] = static_cast<float>(rand_normal(0, 1));
        }
        return result;
    }
}
