/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/**
*/
class BassEnvelopeFilterAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BassEnvelopeFilterAudioProcessorEditor (BassEnvelopeFilterAudioProcessor&);
    ~BassEnvelopeFilterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    static constexpr int mLabelHeight{ 40 };
    static constexpr int mGapHeight{ 40 };
    static constexpr int mBorder{ 10 };

    void addSlider(juce::Slider& slider, std::unique_ptr<SliderAttachment>& attachment, const juce::String& paramID);
    void addLabel(juce::Label& label, const juce::String& text);

    BassEnvelopeFilterAudioProcessor& mAudioProcessor;
    CustomLookAndFeel mLookAndFeel;

    juce::Slider mDrySlider;
    juce::Label mDryLabel;
    std::unique_ptr<SliderAttachment> mDryAttachment;

    juce::Slider mFXSlider;
    juce::Label mFXLabel;
    std::unique_ptr<SliderAttachment> mFXAttachment;

    juce::Slider mDecaySlider;
    juce::Label mDecayLabel;
    std::unique_ptr<SliderAttachment> mDecayAttachment;

    juce::Slider mQSlider;
    juce::Label mQLabel;
    std::unique_ptr<SliderAttachment> mQAttachment;

    juce::Slider mSensSlider;
    juce::Label mSensLabel;
    std::unique_ptr<SliderAttachment> mSensAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BassEnvelopeFilterAudioProcessorEditor)
};
