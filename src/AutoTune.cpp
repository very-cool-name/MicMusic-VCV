#include "AutoTune.hpp"
#include "AutoTuneMatrices.hpp"
#include "Matrix.hpp"

#include <numeric>

namespace math {
    template<class It>
    float find_frequency(It begin, It end, float timespan, float root = 0) {
        float prev_value = root;
        int zeros = 0;
        while (begin != end) {
            if ((*begin - root) * (prev_value - root) < 0) {
                ++zeros;
            }
            prev_value = *begin;
            ++begin;
        }
        return zeros / timespan;
    }
}

AutoTune::AutoTune() 
    : Module((int) Params::COUNT, (int) Inputs::COUNT, (int) Outputs::COUNT, (int) Lights::COUNT)
    , BUFFER_SIZE(1000)
    , values(BUFFER_SIZE)
    , times(BUFFER_SIZE)
    , prevValue(0)
    , inputRow(1, BUFFER_SIZE + 1)
{}

void AutoTune::step() {
    float input = inputs[(int) Inputs::SIGNAL].value;
    times.pop_front();
    times.push_back(engineGetSampleTime());
    values.push_front(input);
    values.pop_back();
    for (int c = 0; c < 1000; ++c)
        inputRow(0, c) = values[c];
    inputRow(0, 1000) = prevValue;
    auto pred = inputRow * i2o;
    outputs[(int) Outputs::SIGNAL].value = pred(0, 0);
    prevValue = pred(0, 0);
    // assert(pred.cols() == 1);
    // assert(pred.rows() == 1);
    // float timespan = std::accumulate(begin(times), end(times), 0.f);
    // float mean = std::accumulate(begin(values), end(values), 0.f) / BUFFER_SIZE;
    // float freq = math::find_frequency(begin(values), end(values), timespan, mean);
    
    // if (freq > 20 && freq < 20000) {
    //     outputs[(int) Outputs::SIGNAL].value = 0;
    // } else {
    //     outputs[(int) Outputs::SIGNAL].value = input;
    // }
}

struct AutoTuneWidget : ModuleWidget {
	AutoTuneWidget(AutoTune *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/Distortion.svg")));

		addChild(Widget::create<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		// addParam(ParamWidget::create<RoundBlackKnob>(Vec(50, 86), module, (int) AutoTune::Params::HIGH, -10.f, 10.f, 10.f));
		// addParam(ParamWidget::create<RoundBlackKnob>(Vec(50, 188), module, (int) AutoTune::Params::HIGH_CV, 0.f, 1.f, 0.f));
        // addInput(Port::create<PJ301MPort>(Vec(52, 149), Port::INPUT, module, (int) AutoTune::Inputs::SIGNAL));
		
        // addParam(ParamWidget::create<RoundBlackKnob>(Vec(10, 86), module, (int) AutoTune::Params::LOW, -10.f, 10.f, -10.f));
        // addParam(ParamWidget::create<RoundBlackKnob>(Vec(10, 188), module, (int) AutoTune::Params::LOW_CV, 0.f, 1.f, 0.f));
        // addInput(Port::create<PJ301MPort>(Vec(12, 149), Port::INPUT, module, (int) AutoTune::Inputs::LOW));

        addInput(Port::create<PJ301MPort>(Vec(12, 330), Port::INPUT, module, (int) AutoTune::Inputs::SIGNAL));
		addOutput(Port::create<PJ301MPort>(Vec(52, 330), Port::OUTPUT, module,(int)  AutoTune::Outputs::SIGNAL));
	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *autoTuneModule = Model::create<AutoTune, AutoTuneWidget>("MicMusic", "AutoTune", "AutoTune - TuNeR", VOCODER_TAG);