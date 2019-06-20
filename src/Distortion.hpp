#pragma once

#include "MicMusic.hpp"

class Distortion : public rack::Module {
public:
    enum class Params {
        HIGH,
        LOW,
        HIGH_CV,
        LOW_CV,
        COUNT
    };

    enum class Inputs {
        HIGH,
        LOW,
        SIGNAL,
        COUNT
    };

    enum class Lights {
        COUNT
    };

    enum class Outputs {
        SIGNAL,
        COUNT
    };
    
    Distortion();

    void step() override;
};


