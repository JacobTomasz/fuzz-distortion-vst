#pragma once

class StateManager;

#include <juce_gui_basics/juce_gui_basics.h>
#include "../parameters/ParameterDefines.h"

class ParameterComboBox : public juce::Component {
public:
    ParameterComboBox(StateManager *s, size_t p_id);
    ~ParameterComboBox() override;

    void resized() override;
    void paint(juce::Graphics &g) override;

private:
    void syncFromState();

    size_t param_id;
    StateManager *state;
    juce::Label   label;
    juce::ComboBox combo;
};
