#pragma once

class LowPassFilter {
public:
    LowPassFilter(float alpha);

    float filter(float y);

    float& alpha() { return alpha_; }

private:
    float alpha_;
    float y_;
};
