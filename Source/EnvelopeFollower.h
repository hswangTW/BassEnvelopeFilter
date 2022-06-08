/*
  ==============================================================================

    Author: Han-Sheng Wang
    Description: Envelope follower implemented with a rectifier and a 1st-order TPT lowpass filter.
                 (For the lowpass, see "The Art of VA Filter Design" written by Vadim Zavalishin)

  ==============================================================================
*/

#pragma once

#define EF_DEFAULT_SAMPLE_RATE 44100
#define EF_DEFAULT_CUTOFF_FREQ 50.0f
// #define EF_MIN_CUTOFF_FREQ 5.0f
// #define EF_MAX_CUTOFF_FREQ 50.0f
#define EF_DEFAULT_OUTPUT_GAIN 1.0f

class EnvelopeFollower
{
public:
    EnvelopeFollower();
    EnvelopeFollower(double sampleRate, float cutoff);
    ~EnvelopeFollower();

    void setSampleRate(double sampleRate);
    void setSens(float sens);
    void setCutoff(float cutoff);
    void setParams(float cutoff, double sampleRate);
    // void setOutputGain(float gain);

    void reset();

    float getNextValue(float input);

private:
    float mSampleRate;

    float mGain;  // The equivalent gain
    float mInput; // The input of the integrator
    float mState; // The state of the integrator

    float mEnvelope;
    float mOutputGain;
};