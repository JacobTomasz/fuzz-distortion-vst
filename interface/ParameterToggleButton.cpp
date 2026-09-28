#include "ParameterToggleButton.h"
#include "../parameters/StateManager.h"

ParameterToggleButton::ParameterToggleButton(StateManager *s, size_t p_id)
    : state(s), param_id(p_id) {
    state->register_component(p_id, this, [this]() { repaint(); });
}

ParameterToggleButton::~ParameterToggleButton() {
    state->unregister_component(param_id, this);
}

bool ParameterToggleButton::isOn() const {
    return state->param_value(param_id) > 0.5f;
}

void ParameterToggleButton::mouseDown(const juce::MouseEvent &e) {
    juce::ignoreUnused(e);
    state->begin_change_gesture(param_id);
    state->set_parameter(param_id, isOn() ? 0.0f : 1.0f);
    state->end_change_gesture(param_id);
    repaint();
}

void ParameterToggleButton::paint(juce::Graphics &g) {
    const bool on = isOn();
    auto b = getLocalBounds();

    // Label at top
    const int label_h = proportionOfHeight(0.38f);
    g.setColour(juce::Colours::black);
    g.drawText(PARAMETER_NICKNAMES[param_id],
               0, 0, getWidth(), label_h,
               juce::Justification::centred, true);

    // Button rect below label
    auto btn = b.withTrimmedTop(label_h).reduced(4);
    g.setColour(on ? juce::Colours::black : juce::Colours::white);
    g.fillRoundedRectangle(btn.toFloat(), 4.0f);
    g.setColour(juce::Colours::black);
    g.drawRoundedRectangle(btn.toFloat(), 4.0f, 1.5f);

    // Button text from TO_STRING_ARR or fallback
    const auto &arr = PARAMETER_TO_STRING_ARRS[param_id];
    const juce::String txt = (arr.size() > 1) ? (on ? arr[1] : arr[0])
                                               : (on ? "On" : "Off");
    g.setColour(on ? juce::Colours::white : juce::Colours::black);
    g.drawText(txt, btn, juce::Justification::centred, true);

    // Outer border
    g.setColour(juce::Colours::black);
    g.drawRect(getLocalBounds());
}
