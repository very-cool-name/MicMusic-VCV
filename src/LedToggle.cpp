#include "LedToggle.hpp"

using namespace rack;

LedToggle::LedToggle() {
    addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/LedToggleOff.svg")));
    addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/LedToggleOn.svg")));
}
