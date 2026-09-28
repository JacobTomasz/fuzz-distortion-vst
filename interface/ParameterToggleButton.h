#pragma once

class StateManager;

#include <juce_gui_basics/juce_gui_basics.h>
#include "../parameters/ParameterDefines.h"

class ParameterToggleButton : public juce::Component {
public:
    ParameterToggleButton(StateManager *s, size_t p_id);
    ~ParameterToggleButton() override;

    void paint(juce::Graphics &g) override;
    void mouseDown(const juce::MouseEvent &e) override;

private:
    bool isOn() const;

    size_t param_id;
    StateManager *state;
};
