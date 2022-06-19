/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 16 Jun 2022 1:37:29pm
    Author:  hanso

  ==============================================================================
*/

#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    auto font = juce::Typeface::createSystemTypefaceFor(CenturyGothicFont::GOTHIC_TTF, CenturyGothicFont::GOTHIC_TTFSize);
    setDefaultSansSerifTypeface(font);

    setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(0xff1c1c1c));
}

CustomLookAndFeel::~CustomLookAndFeel()
{

}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                         float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    auto outline = juce::Colour(0xffffffff);
    auto fill = juce::Colour(0xff434343);
    // auto outline = slider.findColour(juce::Slider::rotarySliderOutlineColourId);
    // auto fill = slider.findColour(juce::Slider::rotarySliderFillColourId);

    auto centre = juce::Point<int>(x + width * 0.5f, y + height * 0.5f).toFloat();
    auto radius = juce::jmin(width - 20, height - 20) / 2;
    auto bounds = juce::Rectangle<int>(2 * radius, 2 * radius).toFloat().withCentre(centre);    
    
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto outlineWidth = juce::jmin(2.0f, radius * 0.2f);
    // auto pointerWidth = juce::jmin(8.0f, radius * 0.5f);
    auto outlineRadius = radius - outlineWidth * 0.5f;
    auto outlineDiameter = outlineRadius * 2;

    g.setColour(fill);
    g.fillEllipse(bounds);

    g.setColour(outline);
    g.drawEllipse(juce::Rectangle<float>(outlineDiameter, outlineDiameter).withCentre(bounds.getCentre()), outlineWidth);

    juce::Point<float> pointerPoint(bounds.getCentreX() + outlineRadius * std::cos(toAngle - juce::MathConstants<float>::halfPi),
                                    bounds.getCentreY() + outlineRadius * std::sin(toAngle - juce::MathConstants<float>::halfPi));
    juce::Path path;
    path.addLineSegment(juce::Line<float>(centre, pointerPoint), outlineWidth);
    g.strokePath(path, juce::PathStrokeType(outlineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // juce::Path backgroundArc;
    // backgroundArc.addCentredArc(bounds.getCentreX(),
    //                             bounds.getCentreY(),
    //                             arcRadius,
    //                             arcRadius,
    //                             0.0f,
    //                             rotaryStartAngle,
    //                             rotaryEndAngle,
    //                             true);
    // 
    // g.setColour(outline);
    // g.strokePath(backgroundArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // if (slider.isEnabled())
    // {
    //     juce::Path valueArc;
    //     valueArc.addCentredArc(bounds.getCentreX(),
    //                            bounds.getCentreY(),
    //                            arcRadius,
    //                            arcRadius,
    //                            0.0f,
    //                            rotaryStartAngle,
    //                            toAngle,
    //                            true);
    // 
    //     g.setColour(fill);
    //     g.strokePath(valueArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    // }

    // auto thumbWidth = lineW * 2.0f;
    // juce::Point<float> thumbPoint(bounds.getCentreX() + arcRadius * std::cos(toAngle - juce::MathConstants<float>::halfPi),
    //                               bounds.getCentreY() + arcRadius * std::sin(toAngle - juce::MathConstants<float>::halfPi));
    // 
    // g.setColour(slider.findColour(juce::Slider::thumbColourId));
    // g.fillEllipse(juce::Rectangle<float>(thumbWidth, thumbWidth).withCentre(thumbPoint));
}
