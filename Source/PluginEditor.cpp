/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BassEnvelopeFilterAudioProcessorEditor::BassEnvelopeFilterAudioProcessorEditor (BassEnvelopeFilterAudioProcessor& p)
    : AudioProcessorEditor (&p), mAudioProcessor (p)
{
    juce::LookAndFeel::setDefaultLookAndFeel(&mLookAndFeel);
    setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(0x000000));

    addSlider(mDrySlider, mDryAttachment, "dry");
    addLabel(mDryLabel, "DRY");

    addSlider(mFXSlider, mFXAttachment, "fx");
    addLabel(mFXLabel, "FX");

    addSlider(mDecaySlider, mDecayAttachment, "decay");
    addLabel(mDecayLabel, "DECAY");

    addSlider(mQSlider, mQAttachment, "q");
    addLabel(mQLabel, "Q");

    addSlider(mSensSlider, mSensAttachment, "sens");
    addLabel(mSensLabel, "SENS.");

#if DEBUG
    // addLabel(mDebugLabel, "Debug");

    mDebugSlider.setRange(76, 5000);
    mDebugSlider.setSkewFactorFromMidPoint(sqrt(76 * 5000));
    mDebugSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    mDebugSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(mDebugSlider);

    mAudioProcessor.addChangeListener(this);
    setSize(mWidth, mHeight + mGapHeight + mSliderHeight);
#else
    setSize(mWidth, mHeight);
#endif // DEBUG
}

BassEnvelopeFilterAudioProcessorEditor::~BassEnvelopeFilterAudioProcessorEditor()
{
}

//==============================================================================
void BassEnvelopeFilterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void BassEnvelopeFilterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto area = getLocalBounds().reduced(mBorder);

    auto labelArea = area.removeFromTop(mLabelHeight);
    auto dryArea = labelArea.removeFromLeft(labelArea.getWidth() / 2).removeFromRight(mSliderHeight);
    mDryLabel.setBounds(dryArea);
    auto fxArea = labelArea.removeFromLeft(mSliderHeight);
    mFXLabel.setBounds(fxArea);

    auto sliderArea = area.removeFromTop(mSliderHeight);
    dryArea = sliderArea.removeFromLeft(sliderArea.getWidth() / 2).removeFromRight(mSliderHeight);
    mDrySlider.setBounds(dryArea);
    fxArea = sliderArea.removeFromLeft(mSliderHeight);
    mFXSlider.setBounds(fxArea);

    area.removeFromTop(mGapHeight);

    #ifdef DEBUG
    auto debugArea = area.removeFromBottom(mSliderHeight);
    mDebugSlider.setBounds(debugArea);
    area.removeFromBottom(mGapHeight);
    #endif

    labelArea = area.removeFromTop(mLabelHeight);
    auto decayArea = labelArea.removeFromLeft(mSliderHeight);
    mDecayLabel.setBounds(decayArea);
    auto qArea = labelArea.removeFromLeft(mSliderHeight);
    mQLabel.setBounds(qArea);
    mSensLabel.setBounds(labelArea);

    sliderArea = area.removeFromTop(mSliderHeight);
    decayArea = sliderArea.removeFromLeft(mSliderHeight);
    mDecaySlider.setBounds(decayArea);
    qArea = sliderArea.removeFromLeft(mSliderHeight);
    mQSlider.setBounds(qArea);
    mSensSlider.setBounds(sliderArea);
}

#ifdef DEBUG
void BassEnvelopeFilterAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster* sender)
{
    // mDebugLabel.setText(juce::String(mAudioProcessor.getCutoffFrequency()), juce::dontSendNotification);
    mDebugSlider.setValue(mAudioProcessor.getCutoffFrequency());
}
#endif DEBUG

void BassEnvelopeFilterAudioProcessorEditor::addSlider(juce::Slider& slider, std::unique_ptr<SliderAttachment>& attachment, const juce::String& paramID)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(slider);
    attachment.reset(new SliderAttachment(mAudioProcessor.getAPVTS(), paramID, slider));
}

void BassEnvelopeFilterAudioProcessorEditor::addLabel(juce::Label& label, const juce::String& text)
{
    label.setFont(18.0f);
    label.setText(text, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}