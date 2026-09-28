// Nathan Blair January 2023

#pragma once

class StateManager;
class ParameterSlider;
class ParameterComboBox;

#include "PluginProcessor.h"

//==============================================================================
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
    explicit AudioPluginAudioProcessorEditor(PluginProcessor &);
    ~AudioPluginAudioProcessorEditor() override;

    void paint(juce::Graphics &) override;
    void resized() override;

private:
    void windowReadyToPaint();

    const int W = 900;
    const int H = 500;

    PluginProcessor &processorRef;
    StateManager *state;

    std::unique_ptr<ParameterSlider>   fuzz_slider;
    std::unique_ptr<ParameterSlider>   tone_slider;
    std::unique_ptr<ParameterSlider>   level_slider;
    std::unique_ptr<ParameterSlider>   drywet_slider;
    std::unique_ptr<ParameterComboBox> type_combo;

    std::unique_ptr<juce::VBlankAttachment> repaint_callback_handler;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
