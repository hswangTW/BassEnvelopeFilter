#include <cmath>
#include <algorithm>
#include "EnvelopeFollower.h"

EnvelopeFollower::EnvelopeFollower()
    : mOutputGain(EF_DEFAULT_OUTPUT_GAIN)
{
    setParams(EF_DEFAULT_CUTOFF_FREQ, EF_DEFAULT_SAMPLE_RATE);
    reset();
}

EnvelopeFollower::EnvelopeFollower(double sampleRate, float cutoff)
    : mOutputGain(EF_DEFAULT_OUTPUT_GAIN)
{
    setParams(cutoff, sampleRate);
    reset();
}

EnvelopeFollower::~EnvelopeFollower()
{

}

void EnvelopeFollower::reset()
{
    mInput = 0;
    mState = 0;
    mEnvelope = 0;
}

void EnvelopeFollower::setSampleRate(double sampleRate)
{
    mSampleRate = static_cast<float> (sampleRate);
}

void EnvelopeFollower::setSens(float sens)
{
    mOutputGain = sens;
}

void EnvelopeFollower::setCutoff(float cutoff)
{
    // Directly set the cutoff frequency
    float g = tan(0.5f * cutoff / mSampleRate);
    mGain = g / (1 + g);
}

void EnvelopeFollower::setParams(float cutoff, double sampleRate)
{
    mSampleRate = static_cast<float> (sampleRate);
    float g = tan(0.5f * cutoff / mSampleRate);
    mGain = g / (1 + g);
}

// void EnvelopeFollower::setOutputGain(float gain)
// {
//     mOutputGain = gain;
// }

float EnvelopeFollower::getNextValue(float input)
{
    mInput = mGain * (abs(input) - mState);
    mEnvelope = mInput + mState;
    mState = mInput + mEnvelope;
    
    return mEnvelope * mOutputGain;
    // return std::min(mEnvelope * mOutputGain, 1.0f);
}