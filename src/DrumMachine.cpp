#include "DrumMachine.hpp"

using namespace rack;

namespace Params {
    enum Params {
        AMP_ATTACK,
        AMP_DECAY,
        AMP_SUSTAIN,
        AMP_RELEASE,
        NOISE_AMP,
        FILTER_ALPHA,
        COUNT
    };
}

namespace Inputs {
    enum Inputs {
        IN_IMPULSE0,
        IN_IMPULSE1,
        IN_IMPULSE2,
        IN_IMPULSE3,
        IN_IMPULSE4,
        IN_IMPULSE5,
        IN_IMPULSE6,
        IN_IMPULSE7,
        COUNT
    };
}

namespace Outputs {
    enum Outputs {
        OUT_SIGNAL0,
        OUT_SIGNAL1,
        OUT_SIGNAL2,
        OUT_SIGNAL3,
        OUT_SIGNAL4,
        OUT_SIGNAL5,
        OUT_SIGNAL6,
        OUT_SIGNAL7,
        COUNT
    };
}

namespace Lights {
    enum Lights {
        COUNT
    };
}

DrumMachine::DrumMachine()
: ampEnvelope_(0, 0, 1, 0)
, noiseGen_()
, filter_(1.f) {
    config(Params::COUNT, Inputs::COUNT, Outputs::COUNT, Lights::COUNT);
    configParam(Params::AMP_ATTACK, 0.f, 1.f, 0.f);
    configParam(Params::AMP_DECAY, 0.f, 1.f, 0.f);
    configParam(Params::AMP_SUSTAIN, 0.f, 1.f, 1.f);
    configParam(Params::AMP_RELEASE, 0.f, 1.f, 0.f);
    configParam(Params::NOISE_AMP, 0.f, 1.f, 0.f);
    configParam(Params::FILTER_ALPHA, 0.f, 1.f, 1.f);
}

void DrumMachine::process(const ProcessArgs &args) {
    ampEnvelope_.attack() = params[Params::AMP_ATTACK].getValue();
    ampEnvelope_.decay() = params[Params::AMP_DECAY].getValue();
    ampEnvelope_.release() = params[Params::AMP_RELEASE].getValue();
    ampEnvelope_.sustain() = params[Params::AMP_SUSTAIN].getValue();

    float in = inputs[Inputs::IN_IMPULSE0].value;

    float signal = 1 + noiseGen_() * params[Params::NOISE_AMP].getValue();
    ampEnvelope_.nextState(in);
    float output = signal * ampEnvelope_.nextSample(args.sampleTime);
    filter_.alpha() = params[Params::FILTER_ALPHA].getValue();
    output = filter_.filter(output);
    outputs[Outputs::OUT_SIGNAL0].value = output;
}

struct DrumMachineWidget : ModuleWidget {
	DrumMachineWidget(DrumMachine *module) {
		setModule(module);
        setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Drums.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

        for (int i = 0; i < 8; ++i) {
            addInput(createInput<PJ301MPort>(Vec(7, 20 + i * 43), module, i));
        }

		for (int i = 0; i < 8; ++i) {
            addOutput(createOutput<PJ301MPort>(Vec(420, 20 + i * 43), module, i));
        }

        // noise
        addParam(createParam<RoundBlackKnob>(Vec(220, 49 + 37), module, Params::NOISE_AMP));

        //filter
        addParam(createParam<RoundBlackKnob>(Vec(300, 49 + 37), module, Params::FILTER_ALPHA));

        // amp adsr
        addParam(createParam<RoundBlackKnob>(Vec(362, 49 + 37), module, Params::AMP_ATTACK));
        addParam(createParam<RoundBlackKnob>(Vec(362, 49 + 80), module, Params::AMP_DECAY));
        addParam(createParam<RoundBlackKnob>(Vec(362, 49 + 123), module, Params::AMP_SUSTAIN));
        addParam(createParam<RoundBlackKnob>(Vec(362, 49 + 166), module, Params::AMP_RELEASE));
	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per pluginInstance, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *drumMachineModule = createModel<DrumMachine, DrumMachineWidget>("DrumMachine");