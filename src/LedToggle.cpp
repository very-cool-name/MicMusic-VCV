#include "LedToggle.hpp"

LedToggle::LedToggle() {
    addFrame(SVG::load(assetPlugin(plugin, "res/LedToggleOff.svg")));
    addFrame(SVG::load(assetPlugin(plugin, "res/LedToggleOn.svg")));
}