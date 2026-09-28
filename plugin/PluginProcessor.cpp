// Nathan Blair June 2023

#include "PluginProcessor.h"
#include "../audio/Fuzz.h"
#include "../parameters/StateManager.h"
#include "PluginEditor.h"

//==============================================================================
PluginProcessor::PluginProcessor() { state = std::make_unique<StateManager>(this); }

PluginProcessor::~PluginProcessor() {}

//==============================================================================
void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    (void)samplesPerBlock;
    fuzz = std::make_unique<Fuzz>(
        float(sampleRate),
        getTotalNumOutputChannels(),
        PARAMETER_DEFAULTS[PARAM::FUZZ]    / 100.0f,
        PARAMETER_DEFAULTS[PARAM::TONE]    / 100.0f,
        PARAMETER_DEFAULTS[PARAM::LEVEL]   / 100.0f,
        PARAMETER_DEFAULTS[PARAM::DRY_WET] / 100.0f,
        int(PARAMETER_DEFAULTS[PARAM::FUZZ_TYPE]));
    should_snap_smoothed_params.store(true);
}

void PluginProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                   juce::MidiBuffer &midiMessages) {
    juce::ScopedNoDenormals noDenormals;

    float *const *bufferPtrs = buffer.getArrayOfWritePointers();
    const int numSamples  = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    const float fuzz_param   = state->param_value(PARAM::FUZZ)    / 100.0f;
    const float tone_param   = state->param_value(PARAM::TONE)    / 100.0f;
    const float level_param  = state->param_value(PARAM::LEVEL)   / 100.0f;
    const float drywet_param = state->param_value(PARAM::DRY_WET) / 100.0f;
    const int   fuzz_type    = int(std::round(state->param_value(PARAM::FUZZ_TYPE)));

    if (should_snap_smoothed_params.exchange(false))
        fuzz->setState(fuzz_param, tone_param, level_param, drywet_param, fuzz_type);

    fuzz->process(bufferPtrs, numSamples, numChannels,
                  fuzz_param, tone_param, level_param, drywet_param, fuzz_type);

    midiMessages.clear();
}

void PluginProcessor::reset() {
    should_snap_smoothed_params.store(true);
}

//==============================================================================
void PluginProcessor::getStateInformation(juce::MemoryBlock &destData) {
    auto plugin_state = state->get_state();
    std::unique_ptr<juce::XmlElement> xml(plugin_state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PluginProcessor::setStateInformation(const void *data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    state->load_from(xmlState.get());
    should_snap_smoothed_params.store(true);
}

juce::AudioProcessorEditor *PluginProcessor::createEditor() {
    return new AudioPluginAudioProcessorEditor(*this);
}

//==============================================================================
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new PluginProcessor(); }
