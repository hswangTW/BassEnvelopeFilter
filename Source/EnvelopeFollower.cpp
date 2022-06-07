#include <cmath>
#include "EnvelopeFollower.h"

EnvelopeFollower::EnvelopeFollower()
{
    mEnvelope = 0;
}

EnvelopeFollower::EnvelopeFollower(double sampleRate, float attack, float release)
{
    setSampleRate(sampleRate);
    setParams(sampleRate, attack, release);
    mEnvelope = 0;
}

EnvelopeFollower::~EnvelopeFollower()
{

}

void EnvelopeFollower::reset()
{
    mEnvelope = 0;
}

void EnvelopeFollower::setSampleRate(double sampleRate)
{
    mSampleRate = static_cast<float> (sampleRate);
}

void EnvelopeFollower::setAttack(float attack)
{
    mAttackCoeff = pow(0.01f, 1000 / attack / mSampleRate);
}

void EnvelopeFollower::setRelease(float release)
{
    mReleaseCoeff = pow(0.01f, 1000 / release / mSampleRate);
}

void EnvelopeFollower::setParams(float attack, float release, double sampleRate)
{
    mSampleRate = static_cast<float> (sampleRate);
    mAttackCoeff = pow(0.01f, 1000 / attack / mSampleRate);
    mReleaseCoeff = pow(0.01f, 1000 / release / mSampleRate);
}

float EnvelopeFollower::getNextValue(float input)
{
    mRectifiedInput = abs(input);
    if (mRectifiedInput > mEnvelope)
        mEnvelope = mRectifiedInput + mAttackCoeff * (mEnvelope - mRectifiedInput);
    else
        mEnvelope = mRectifiedInput + mReleaseCoeff * (mEnvelope - mRectifiedInput);
    
    return mEnvelope;
}