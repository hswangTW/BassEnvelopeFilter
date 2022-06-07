/*
  ==============================================================================

    Author: Han-Sheng Wang
    Description: My implementation of a TPT state variable filter.
                 (Vadim Zavalishin. "The Art of VA Filter Design")

  ==============================================================================
*/

#pragma once

#define DEFAULT_SAMPLE_RATE 44100
#define MIN_CUTOFF_FREQ 76.0f
#define MAX_CUTOFF_FREQ 3200.0f
#define DEFAULT_Q 0.5f

class StateVariableFilter
{
public:
    StateVariableFilter();
    StateVariableFilter(double sampleRate, float freq, float q);
    ~StateVariableFilter();

    void setSampleRate(double sampleRate);
    void setCutoffFrequency(float freq);
    void setParams(float freq, float q);
    void setParams(float freq, float q, double sampleRate);
    void setResonance(float q);

    void reset();

    float getNextValue(float input);

private:
    float mSampleRate;

    float mGain; // The gain of each integrator
    float mDamping; // 1 / (2 * Q)
    float mDenominator; // It's actually the reciprocal of the denominator

    float mCutoff;

    // The (amplified) inputs and states of the integrators
    float mInput1;
    float mInput2;
    float mState1;
    float mState2;

    float mBandPass;
    float mLowPass;
};