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

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 320);
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
    int sliderWidth = area.getWidth() / 3;

    auto upper = area.removeFromTop((area.getHeight() - mGapHeight) / 2);
    auto dryArea = upper.removeFromLeft(area.getWidth() / 2).removeFromRight(sliderWidth);
    mDryLabel.setBounds(dryArea.removeFromTop(mLabelHeight));
    mDrySlider.setBounds(dryArea);

    auto fxArea = upper.removeFromLeft(sliderWidth);
    mFXLabel.setBounds(fxArea.removeFromTop(mLabelHeight));
    mFXSlider.setBounds(fxArea);

    area.removeFromTop(mGapHeight);
    auto decayArea = area.removeFromLeft(sliderWidth);
    mDecayLabel.setBounds(decayArea.removeFromTop(mLabelHeight));
    mDecaySlider.setBounds(decayArea);

    auto qArea = area.removeFromLeft(sliderWidth);
    mQLabel.setBounds(qArea.removeFromTop(mLabelHeight));
    mQSlider.setBounds(qArea);

    auto sensArea = area;
    mSensLabel.setBounds(sensArea.removeFromTop(mLabelHeight));
    mSensSlider.setBounds(sensArea);
}

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