#include "Distortion.hpp"

using namespace rack;

Distortion::Distortion() {
    config((int) Params::COUNT, (int) Inputs::COUNT, (int) Outputs::COUNT, (int) Lights::COUNT);
    configParam((int) Distortion::Params::HIGH, -10.f, 10.f, 10.f);
    configParam((int) Distortion::Params::HIGH_CV, 0.f, 1.f, 0.f);
    configParam((int) Distortion::Params::LOW, -10.f, 10.f, -10.f);
    configParam((int) Distortion::Params::LOW_CV, 0.f, 1.f, 0.f);
}

void Distortion::process(const ProcessArgs &args) {
    float input = inputs[(int) Inputs::SIGNAL].getVoltage();
    float high_cv = inputs[(int) Inputs::HIGH].getVoltage() * params[(int) Params::HIGH_CV].getValue();
    float high_value = params[(int) Params::HIGH].getValue() + high_cv;
    float low_cv = inputs[(int) Inputs::LOW].getVoltage() * params[(int) Params::LOW_CV].getValue();
    float low_value = params[(int) Params::LOW].getValue() + low_cv;
	
    //_debugOut << params[(int) Inputs::HIGH].getValue();
    outputs[(int) Outputs::SIGNAL].setVoltage(rack::math::clamp(input, low_value, high_value));
}

struct DistortionWidget : ModuleWidget {
	DistortionWidget(Distortion *module) {
		setModule(module);
        setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Distortion.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParam<RoundBlackKnob>(Vec(50, 86), module, (int) Distortion::Params::HIGH));
		addParam(createParam<RoundBlackKnob>(Vec(50, 188), module, (int) Distortion::Params::HIGH_CV));
        addInput(createInput<PJ301MPort>(Vec(53, 150), module, (int) Distortion::Inputs::HIGH));
		
        addParam(createParam<RoundBlackKnob>(Vec(10, 86), module, (int) Distortion::Params::LOW));
        addParam(createParam<RoundBlackKnob>(Vec(10, 188), module, (int) Distortion::Params::LOW_CV));
        addInput(createInput<PJ301MPort>(Vec(13, 150), module, (int) Distortion::Inputs::LOW));

        addInput(createInput<PJ301MPort>(Vec(13, 331), module, (int) Distortion::Inputs::SIGNAL));
		addOutput(createOutput<PJ301MPort>(Vec(53, 331), module,(int)  Distortion::Outputs::SIGNAL));
	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per pluginInstance, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *distortionModule = createModel<Distortion, DistortionWidget>("Cuter");
