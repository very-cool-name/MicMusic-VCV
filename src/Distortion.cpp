#include "Distortion.hpp"

using namespace rack;

Distortion::Distortion() 
    : Module((int) Params::COUNT, (int) Inputs::COUNT, (int) Outputs::COUNT, (int) Lights::COUNT)
{}

void Distortion::step() {
    float input = inputs[(int) Inputs::SIGNAL].value;
    float high_cv = inputs[(int) Inputs::HIGH].value * params[(int) Params::HIGH_CV].value;
    float high_value = params[(int) Params::HIGH].value + high_cv;
    float low_cv = inputs[(int) Inputs::LOW].value * params[(int) Params::LOW_CV].value;
    float low_value = params[(int) Params::LOW].value + low_cv;
	
    //_debugOut << params[(int) Inputs::HIGH].value;
    outputs[(int) Outputs::SIGNAL].value = rack::math::clamp(input, low_value, high_value);
}

struct DistortionWidget : ModuleWidget {
	DistortionWidget(Distortion *module) {
		setModule(module);
        setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Distortion.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParam<RoundBlackKnob>(Vec(50, 86), module, (int) Distortion::Params::HIGH, -10.f, 10.f, 10.f));
		addParam(createParam<RoundBlackKnob>(Vec(50, 188), module, (int) Distortion::Params::HIGH_CV, 0.f, 1.f, 0.f));
        addInput(createPort<PJ301MPort>(Vec(52, 149), PortWidget::INPUT, module, (int) Distortion::Inputs::HIGH));
		
        addParam(createParam<RoundBlackKnob>(Vec(10, 86), module, (int) Distortion::Params::LOW, -10.f, 10.f, -10.f));
        addParam(createParam<RoundBlackKnob>(Vec(10, 188), module, (int) Distortion::Params::LOW_CV, 0.f, 1.f, 0.f));
        addInput(createPort<PJ301MPort>(Vec(12, 149), PortWidget::INPUT, module, (int) Distortion::Inputs::LOW));

        addInput(createPort<PJ301MPort>(Vec(12, 330), PortWidget::INPUT, module, (int) Distortion::Inputs::SIGNAL));
		addOutput(createPort<PJ301MPort>(Vec(52, 330), PortWidget::OUTPUT, module,(int)  Distortion::Outputs::SIGNAL));
	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per pluginInstance, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *distortionModule = createModel<Distortion, DistortionWidget>("Cuter");
