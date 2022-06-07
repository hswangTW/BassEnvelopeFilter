/*
  ==============================================================================

    Author: Han-Sheng Wang
    Description: Envelope follower

  ==============================================================================
*/

#pragma once

class EnvelopeFollower
{
public:
    EnvelopeFollower();
    EnvelopeFollower(double sampleRate, float attack, float release);
    ~EnvelopeFollower();

    void setSampleRate(double sampleRate);
    void setAttack(float attack);
    void setRelease(float release);
    void setParams(float attack, float release, double sampleRate);

    void reset();

    float getNextValue(float input);

private:
    float mSampleRate;
    float mAttackCoeff;
    float mReleaseCoeff;

    float mEnvelope;

    float mRectifiedInput;
};