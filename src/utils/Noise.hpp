#include <random>

class NoiseGenerator {
public:
    NoiseGenerator(std::mt19937::result_type seed): gen_(seed) {};

    virtual float operator()() = 0;

protected:
    std::mt19937 gen_;
};

class WhiteNoiseGenerator : NoiseGenerator {
public:
    WhiteNoiseGenerator(std::mt19937::result_type seed = std::random_device()()): NoiseGenerator(seed), distr_(0.5) {};
    float operator()() override  {
        return distr_(gen_) ? 1 : -1;
    }

private:
    std::bernoulli_distribution distr_;
};