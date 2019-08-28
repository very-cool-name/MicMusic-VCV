#pragma once

#include "MicMusic.hpp"

#include "util/CircularBuffer.hpp"

class Delay : public rack::Module {
public:
    Delay();

    void process(const ProcessArgs &args) override;

private:
    using Buffer = CircularBuffer<float>;
    static const Buffer::Size maxDelay_;
    Buffer buffer_;
};