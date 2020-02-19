#pragma once

#include "MicMusic.hpp"

#include "utils/EnvelopeGenerator.hpp"
#include "utils/LowPassFilter.hpp"
#include "utils/Noise.hpp"

class DrumMachine : public rack::Module {
public:
    DrumMachine();
    void process(const ProcessArgs &args) override;

private:
    EnvelopeGenerator ampEnvelope_;
    WhiteNoiseGenerator noiseGen_;
    LowPassFilter filter_;
};
