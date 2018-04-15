#include <numeric>
#include <stdexcept>
#include <vector>

template <typename T>
struct MeanAndStandardDeviation
{
    MeanAndStandardDeviation(T mean, T standard_deviation)
        : mean(mean), standard_deviation(standard_deviation)
    {}

    const T mean;
    const T standard_deviation;
};

template <typename T>
MeanAndStandardDeviation<double> calculate_mean_and_standard_deviation(
    const std::vector<T>& values, // the main input data - TODO: use a generic type, not just vector
    bool unbiased = true, // apply Bessel's correction? (see: https://en.wikipedia.org/wiki/Bessel%27s_correction)
    std::vector<T>& temp = std::vector<T>() // can be passed to avoid unnecessary memory re-allocations
)
{
    const unsigned int bessel_correction = unbiased ? 1 : 0;

    if (values.size() < 1 + bessel_correction) {
        throw std::runtime_error("calculate_mean_and_standard_deviation: the input vector is too short");
    }

    const double mean = std::accumulate(values.begin(), values.end(), 0.0) / values.size();

    temp.resize(values.size());
    std::transform(values.begin(), values.end(), temp.begin(), [mean](T x) { return x - mean; });

    const double squared_sum = std::inner_product(temp.begin(), temp.end(), temp.begin(), 0.0);
    const double standard_deviation = std::sqrt(squared_sum / (values.size() - bessel_correction));

    return MeanAndStandardDeviation<double>(mean, standard_deviation);
}
