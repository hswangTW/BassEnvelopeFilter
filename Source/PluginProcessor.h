/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <array>
#include <JuceHeader.h>
#include "EnvelopeFollower.h"
#include "StateVariableFilter.h"

//==============================================================================
/**
*/

class BassEnvelopeFilterAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    BassEnvelopeFilterAudioProcessor();
    ~BassEnvelopeFilterAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BassEnvelopeFilterAudioProcessor)

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // float mGainRelease;
    // std::array<float, 2> mEnvelopes;

    juce::AudioProcessorValueTreeState mParameters;
    std::atomic<float>* mAttackParameter = nullptr;
    std::atomic<float>* mReleaseParameter = nullptr;
    std::atomic<float>* mQParameter = nullptr;
    std::atomic<float>* mDepthParameter = nullptr;
    std::atomic<float>* mDrywetParameter = nullptr;

    std::vector<std::unique_ptr<EnvelopeFollower>> mEnvelopeFollowers;
    std::vector<std::unique_ptr<StateVariableFilter>> mFilters;

    // juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> mSmoothedQ;
    // juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> mSmoothedDepth;
};