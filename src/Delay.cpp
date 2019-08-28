#include "Delay.hpp"
#include "LedToggle.hpp"

#include <cmath>

using namespace rack;

namespace Params{
enum Params {
    DELAY_SAMPLES,
    DRY_WET,
    COUNT
};
}

namespace Inputs{
enum Inputs {
    SIGNAL,
    COUNT
};
}

namespace Outputs{
enum Outputs {
    SIGNAL,
    COUNT
};
}

namespace Lights{
enum Lights {
    COUNT
};
}

const Delay::Buffer::Size Delay::maxDelay_ = 44100;

Delay::Delay(): buffer_(maxDelay_) {
    config(Params::COUNT, Inputs::COUNT, Outputs::COUNT, Lights::COUNT);
    configParam(Params::DELAY_SAMPLES, 1.f, maxDelay_, 1.f);
    configParam(Params::DRY_WET, 0.f, 1.f, 0.f);
}

void Delay::process(const ProcessArgs &args) {
    float& output = outputs[Outputs::SIGNAL].value;
    const float input = inputs[Inputs::SIGNAL].value;
    float delayed = 0;
    if(buffer_.full()) {
        delayed = buffer_.read();
        buffer_.pop();
    }
    buffer_.push(input);
    const float dryWet = params[Params::DRY_WET].getValue();
    output = (1 - dryWet) * input + delayed * dryWet;

    Buffer::Size newSize = static_cast<Buffer::Size>(std::floor(params[Params::DELAY_SAMPLES].getValue()));
    if (buffer_.bufferSize() != newSize) {
        buffer_.resize(newSize);
    }
}


struct DelayWidget : ModuleWidget {
	DelayWidget(Delay *module) {
		setModule(module);
        setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Adder.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

        addInput(createInput<PJ301MPort>(Vec(7, 49), module, Inputs::SIGNAL));
		addOutput(createOutput<PJ301MPort>(Vec(58, 344), module, Outputs::SIGNAL));

        addParam(createParam<RoundBlackKnob>(Vec(42, 46), module, Params::DELAY_SAMPLES));
        addParam(createParam<RoundBlackKnob>(Vec(42, 76), module, Params::DRY_WET));
	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per pluginInstance, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *delayModule = createModel<Delay, DelayWidget>("Delay");
