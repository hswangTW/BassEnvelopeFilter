#include <cmath>
#include "StateVariableFilter.h"

StateVariableFilter::StateVariableFilter()
{
    setParams(DEFAULT_SAMPLE_RATE, MIN_CUTOFF_FREQ, DEFAULT_Q);
    reset();
}

StateVariableFilter::StateVariableFilter(double sampleRate, float freq = MIN_CUTOFF_FREQ, float q = DEFAULT_Q)
{
    setParams(sampleRate, freq, q);
    reset();
}

StateVariableFilter::~StateVariableFilter()
{
    
}

void StateVariableFilter::reset()
{
    mInput1 = 0;
    mInput2 = 0;
    mState1 = 0;
    mState2 = 0;
    mBandPass = 0;
    mLowPass = 0;
}

void StateVariableFilter::setSampleRate(double sampleRate)
{
    mSampleRate = static_cast<float> (sampleRate);
}

void StateVariableFilter::setCutoffFrequency(float normalizedFreq)
{
    mCutoff = MIN_CUTOFF_FREQ + normalizedFreq * (MAX_CUTOFF_FREQ - MIN_CUTOFF_FREQ);
    mGain = tan(0.5f * mCutoff / mSampleRate);
    mDenominator = 1 / (1 + 2 * mDamping * mGain + mGain * mGain);
}

void StateVariableFilter::setResonance(float q)
{
    mDamping = 0.5f / q;
    mDenominator = 1 / (1 + 2 * mDamping * mGain + mGain * mGain);
}

void StateVariableFilter::setParams(float normalizedFreq, float q)
{
    mCutoff = MIN_CUTOFF_FREQ + normalizedFreq * (MAX_CUTOFF_FREQ - MIN_CUTOFF_FREQ);
    mGain = tan(0.5f * mCutoff / mSampleRate);
    mDamping = 0.5f / q;
    mDenominator = 1 / (1 + 2 * mDamping * mGain + mGain * mGain);
}

void StateVariableFilter::setParams(float normalizedFreq, float q, double sampleRate)
{
    setSampleRate(sampleRate);
    setParams(normalizedFreq, q);
}

float StateVariableFilter::getNextValue(float input)
{
    mBandPass = (mGain * (input - mState2) + mState1) * mDenominator;
    mInput1 = mBandPass - mState1;
    mState1 = mInput1 + mBandPass;

    mInput2 = mGain * mBandPass;
    mLowPass = mInput2 + mState2;
    mState2 = mInput2 + mLowPass;

    // For this envelope filter, only the lowpass mode is used
    return mLowPass;
}