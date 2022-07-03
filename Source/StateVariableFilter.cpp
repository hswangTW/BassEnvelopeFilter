#define _USE_MATH_DEFINES
#include <cmath>
#include "StateVariableFilter.h"

StateVariableFilter::StateVariableFilter()
    : mMinCutoff(SVF_MIN_CUTOFF_FREQ)
{
    setParams(SVF_MIN_CUTOFF_FREQ, SVF_DEFAULT_Q, SVF_DEFAULT_SAMPLE_RATE);
    reset();
}

StateVariableFilter::StateVariableFilter(double sampleRate, float freq = SVF_MIN_CUTOFF_FREQ, float q = SVF_DEFAULT_Q)
    : mMinCutoff(freq)
{
    setParams(freq, q, sampleRate);
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
    mCutoff = mMinCutoff + normalizedFreq * (SVF_MAX_CUTOFF_FREQ - mMinCutoff);
    mGain = tan(M_PI * mCutoff / mSampleRate);
    mDenominator = 1 / (1 + 2 * mDamping * mGain + mGain * mGain);
}

void StateVariableFilter::setResonance(float q)
{
    mDamping = 0.5f / q;
    mDenominator = 1 / (1 + 2 * mDamping * mGain + mGain * mGain);
}

void StateVariableFilter::setParams(float normalizedFreq, float q)
{
    mCutoff = mMinCutoff + normalizedFreq * (SVF_MAX_CUTOFF_FREQ - mMinCutoff);
    mGain = tan(M_PI * mCutoff / mSampleRate);
    mDamping = 0.5f / q;
    mDenominator = 1 / (1 + 2 * mDamping * mGain + mGain * mGain);
}

void StateVariableFilter::setParams(float normalizedFreq, float q, double sampleRate)
{
    setSampleRate(sampleRate);
    setParams(normalizedFreq, q);
}

void StateVariableFilter::setMinCutoffFrequency(float freq)
{
    mMinCutoff = freq;
}

float StateVariableFilter::getCutoffFrequency() const
{
    return mCutoff;
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