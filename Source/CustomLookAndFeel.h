/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 16 Jun 2022 1:37:16pm
    Author:  hanso

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Resources/CenturyGothicFont.h"

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();
    ~CustomLookAndFeel();

    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
                          float sliderPos, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider&) override;
};