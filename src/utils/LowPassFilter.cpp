#include "LowPassFilter.hpp"

LowPassFilter::LowPassFilter(float alpha)
: alpha_(alpha)
, y_(0) {}

float LowPassFilter::filter(float y) {
    float out = y_ + alpha_ * (y - y_);
    y_ = out;
    return out;
}
