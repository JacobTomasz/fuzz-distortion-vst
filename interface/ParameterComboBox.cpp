#include "ParameterComboBox.h"
#include "../parameters/StateManager.h"

ParameterComboBox::ParameterComboBox(StateManager *s, size_t p_id)
    : state(s), param_id(p_id) {

    label.setText(PARAMETER_NICKNAMES[p_id], juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(label);

    const auto &arr = PARAMETER_TO_STRING_ARRS[p_id];
    for (int i = 0; i < (int)arr.size(); ++i)
        combo.addItem(arr[i], i + 1); // JUCE combo IDs are 1-indexed

    combo.setColour(juce::ComboBox::backgroundColourId, juce::Colours::white);
    combo.setColour(juce::ComboBox::textColourId,       juce::Colours::black);
    combo.setColour(juce::ComboBox::outlineColourId,    juce::Colours::black);
    combo.setColour(juce::ComboBox::arrowColourId,      juce::Colours::black);

    combo.onChange = [this]() {
        state->begin_change_gesture(param_id);
        state->set_parameter(param_id, float(combo.getSelectedItemIndex()));
        state->end_change_gesture(param_id);
    };
    addAndMakeVisible(combo);

    syncFromState();
    state->register_component(p_id, this, [this]() { syncFromState(); repaint(); });
}

ParameterComboBox::~ParameterComboBox() {
    state->unregister_component(param_id, this);
}

void ParameterComboBox::syncFromState() {
    const int idx = int(std::round(state->param_value(param_id)));
    combo.setSelectedItemIndex(idx, juce::dontSendNotification);
}

void ParameterComboBox::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::white);
    g.setColour(juce::Colours::black);
    g.drawRect(getLocalBounds());
}

void ParameterComboBox::resized() {
    auto b = getLocalBounds().reduced(2);
    label.setBounds(b.removeFromTop(proportionOfHeight(0.38f)));
    combo.setBounds(b.reduced(2, 0));
}
