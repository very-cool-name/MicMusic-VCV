#include "Adder.hpp"
#include "LedToggle.hpp"

using namespace rack;

Adder::Adder() 
    : Module((int) Params::COUNT, (int) Inputs::COUNT, (int) Outputs::COUNT, (int) Lights::COUNT)
{}

void Adder::step() {
    float& output = outputs[(int) Outputs::SIGNAL].value;
    output = 0;
    if (params[(int) Params::MUTE].value > 0)
        return;
    const int inputs_count = (int) Inputs::COUNT;
    for (int i = 0; i < inputs_count; ++i) {
        const bool mute = params[i + inputs_count * 2].value > 0;
        if (mute)
            continue;
        int sign = (params[i + inputs_count].value > 0) ? 1 : -1;
        if (inputs[i].active) {
            output += inputs[i].value * params[i].value * sign;
        } else {
            output += params[i].value * sign;
        }
    }
}

struct AdderWidget : ModuleWidget {
	AdderWidget(Adder *module) {
		setModule(module);
        setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Adder.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

        for (int i = 0; i < (int) Adder::Inputs::COUNT; ++i) {
            addInput(createPort<PJ301MPort>(Vec(6, 50 + i * 43), PortWidget::INPUT, module, i));
            addParam(createParam<RoundBlackKnob>(Vec(42, 46 + i * 43), module, i, 0.f, 1.f, 1.f));
            addParam(createParam<CKSS>(Vec(87, 53 + i * 43), module, static_cast<int>(Adder::Inputs::COUNT) + i, 0.f, 1.f, 1.f));
            addParam(createParam<LedToggle>(Vec(106, 58 + i * 43), module, 2 * static_cast<int>(Adder::Inputs::COUNT) + i, 0.f, 1.f, 1.f));
        }

        addParam(createParam<LedToggle>(Vec(38, 351), module, (int) Adder::Params::MUTE, 0.f, 1.f, 1.f));
		addOutput(createPort<PJ301MPort>(Vec(57, 344), PortWidget::OUTPUT, module,(int)  Adder::Outputs::SIGNAL));
	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per pluginInstance, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *adderModule = createModel<Adder, AdderWidget>("Seven");
