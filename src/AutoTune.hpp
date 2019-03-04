#pragma once

#include "MicMusic.hpp"

#include "Matrix.hpp"

#include <deque>

class AutoTune : public Module {
public:
    enum class Params {
        // TIME,
        COUNT
    };

    enum class Inputs {
        SIGNAL,
        COUNT
    };

    enum class Lights {
        // FREQ,
        COUNT
    };

    enum class Outputs {
        SIGNAL,
        COUNT
    };
    
    AutoTune();

    void step() override;

private:
    const int BUFFER_SIZE;
    std::deque<float> values;
    std::deque<float> times;
    float prevValue;
    Matrix inputRow;
};


