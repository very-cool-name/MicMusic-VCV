#include "Adder.hpp"

Adder::Adder() 
    : Module((int) Params::COUNT, (int) Inputs::COUNT, (int) Outputs::COUNT, (int) Lights::COUNT)
{}

void Adder::step() {
    float& output = outputs[(int) Outputs::SIGNAL].value;
    output = 0;
    lights[(int) Lights::MUTE].value = params[(int) Params::MUTE].value;
    if (params[(int) Params::MUTE].value < 1)
        return;
    const int inputs_count = (int) Inputs::COUNT;
    for (int i = 0; i < inputs_count; ++i) {
        int sign = (params[i + inputs_count].value > 0) ? 1 : -1;
        if (inputs[i].active) {
            output += inputs[i].value * params[i].value * sign;
        } else {
            output += params[i].value * sign;
        }
    }
}

struct AdderWidget : ModuleWidget {
	AdderWidget(Adder *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/Adder.svg")));

		addChild(Widget::create<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

        for (int i = 0; i < (int) Adder::Inputs::COUNT; ++i) {
            addInput(Port::create<PJ301MPort>(Vec(6, 50 + i * 43), Port::INPUT, module, i));
            addParam(ParamWidget::create<RoundBlackKnob>(Vec(42, 46 + i * 43), module, i, 0.f, 2.f, 0.f));
            addParam(ParamWidget::create<CKSS>(Vec(87, 50 + i * 43), module, static_cast<int>(Adder::Inputs::COUNT) + i, 0.f, 1.f, 1.f)); 
        }

        addParam(ParamWidget::create<CKSS>(Vec(34, 346), module, (int) Adder::Params::MUTE, 0.f, 1.f, 1.f));
		addOutput(Port::create<PJ301MPort>(Vec(57, 344), Port::OUTPUT, module,(int)  Adder::Outputs::SIGNAL));
        addChild(ModuleLightWidget::create<SmallLight<RedLight>>(Vec(16, 352), module, (int) Adder::Lights::MUTE));
	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *adderModule = Model::create<Adder, AdderWidget>("MicMusic", "Adder", "Adder - SeVeN", MIXER_TAG);