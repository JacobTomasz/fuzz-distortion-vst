// Nathan Blair June 2023

#pragma once

class StateManager;
class Fuzz;

#include <juce_audio_basics/juce_audio_basics.h>

#include "PluginProcessorBase.h"
#include <atomic>

//==============================================================================
class PluginProcessor : public PluginProcessorBase {
public:
  //==============================================================================
  PluginProcessor();
  ~PluginProcessor() override;
  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
  void reset() override;
  //==============================================================================
  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;
  //==============================================================================
  juce::AudioProcessorEditor *createEditor() override;
  //==============================================================================
  // state
  //==============================================================================
  std::unique_ptr<StateManager> state;

private:
  std::unique_ptr<Fuzz> fuzz;

  std::atomic<bool> should_snap_smoothed_params{true};

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
