// Nathan Blair January 2023

#include "PluginEditor.h"
#include "../interface/ParameterSlider.h"
#include "../interface/ParameterComboBox.h"
#include "../parameters/StateManager.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(PluginProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p) {
    state = processorRef.state.get();

    fuzz_slider   = std::make_unique<ParameterSlider>(state, PARAM::FUZZ);
    tone_slider   = std::make_unique<ParameterSlider>(state, PARAM::TONE);
    level_slider  = std::make_unique<ParameterSlider>(state, PARAM::LEVEL);
    drywet_slider = std::make_unique<ParameterSlider>(state, PARAM::DRY_WET);
    type_combo    = std::make_unique<ParameterComboBox>(state, PARAM::FUZZ_TYPE);

    addAndMakeVisible(*fuzz_slider);
    addAndMakeVisible(*tone_slider);
    addAndMakeVisible(*level_slider);
    addAndMakeVisible(*drywet_slider);
    addAndMakeVisible(*type_combo);

    setOpaque(true);
    setSize(W, H);
    setColour(0, juce::Colours::white);

    setResizable(true, true);
    setResizeLimits((W * 4) / 5, (H * 4) / 5, (W * 3) / 2, (H * 3) / 2);
    getConstrainer()->setFixedAspectRatio(float(W) / float(H));

    repaint_callback_handler =
        std::make_unique<juce::VBlankAttachment>(this, [this](double) { windowReadyToPaint(); });
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics &g) {
    g.fillAll(findColour(0));
}

void AudioPluginAudioProcessorEditor::resized() {
    const int knob_size = proportionOfWidth(0.10f);
    const int knob_gap  = proportionOfWidth(0.025f);
    const int row_w     = 4 * knob_size + 3 * knob_gap;
    const int row_x     = (getWidth() - row_w) / 2;
    const int row_y     = proportionOfHeight(0.52f); // shifted down to give room above

    fuzz_slider  ->setBounds(row_x,                               row_y, knob_size, knob_size);
    tone_slider  ->setBounds(row_x + knob_size + knob_gap,        row_y, knob_size, knob_size);
    level_slider ->setBounds(row_x + 2 * (knob_size + knob_gap),  row_y, knob_size, knob_size);
    drywet_slider->setBounds(row_x + 3 * (knob_size + knob_gap),  row_y, knob_size, knob_size);

    // Dropdown above the fuzz knob, centered on it
    const int combo_w = knob_size + proportionOfWidth(0.04f);
    const int combo_h = proportionOfHeight(0.14f);
    const int combo_x = row_x + knob_size / 2 - combo_w / 2;
    const int combo_y = row_y - combo_h - proportionOfHeight(0.04f);

    type_combo->setBounds(combo_x, combo_y, combo_w, combo_h);
}

void AudioPluginAudioProcessorEditor::windowReadyToPaint() {
    if (state->any_parameter_changed.exchange(false)) {
        for (size_t param_id{0}; param_id < TOTAL_NUMBER_PARAMETERS; ++param_id) {
            if (state->get_parameter_modified(param_id)) {
                for (const auto &[component, callback_fn] : state->get_callbacks(param_id))
                    callback_fn();
            }
        }
    }
    state->update_preset_modified();
}
