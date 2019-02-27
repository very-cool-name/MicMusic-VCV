#include "Distortion.hpp"

#include <cmath>

namespace math {
float clamp(float value, float min, float max) {
    return std::max(std::min(value, max), min);
}
}

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
    outputs[(int) Outputs::SIGNAL].value = math::clamp(input, low_value, high_value);
}

struct DistortionWidget : ModuleWidget {
	DistortionWidget(Distortion *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/Distortion.svg")));

		addChild(Widget::create<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(ParamWidget::create<RoundBlackKnob>(Vec(50, 86), module, (int) Distortion::Params::HIGH, -10.f, 10.f, 10.f));
		addParam(ParamWidget::create<RoundBlackKnob>(Vec(50, 188), module, (int) Distortion::Params::HIGH_CV, 0.f, 1.f, 0.f));
        addInput(Port::create<PJ301MPort>(Vec(52, 149), Port::INPUT, module, (int) Distortion::Inputs::HIGH));
		
        addParam(ParamWidget::create<RoundBlackKnob>(Vec(10, 86), module, (int) Distortion::Params::LOW, -10.f, 10.f, -10.f));
        addParam(ParamWidget::create<RoundBlackKnob>(Vec(10, 188), module, (int) Distortion::Params::LOW_CV, 0.f, 1.f, 0.f));
        addInput(Port::create<PJ301MPort>(Vec(12, 149), Port::INPUT, module, (int) Distortion::Inputs::LOW));

        addInput(Port::create<PJ301MPort>(Vec(12, 330), Port::INPUT, module, (int) Distortion::Inputs::SIGNAL));
		addOutput(Port::create<PJ301MPort>(Vec(52, 330), Port::OUTPUT, module,(int)  Distortion::Outputs::SIGNAL));
	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *distortionModule = Model::create<Distortion, DistortionWidget>("MicMusic", "Distortion", "Distortion - CuTeR", DISTORTION_TAG);
