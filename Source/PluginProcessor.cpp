/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BassEnvelopeFilterAudioProcessor::BassEnvelopeFilterAudioProcessor()
    : AudioProcessor(BusesProperties()
        #if ! JucePlugin_IsMidiEffect
        #if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        #endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
        #endif
    ),
      mParameters(*this, nullptr, juce::Identifier("BassEnvFilter"), createParameterLayout())
{
    mAttackParameter = mParameters.getRawParameterValue("attack");
    mReleaseParameter = mParameters.getRawParameterValue("release");
    mQParameter = mParameters.getRawParameterValue("q_factor");
    mDepthParameter = mParameters.getRawParameterValue("depth");
    mDrywetParameter = mParameters.getRawParameterValue("drywet");

    for (int i = 0; i < getTotalNumInputChannels(); ++i) {
        mEnvelopeFollowers.push_back( std::make_unique<EnvelopeFollower>() );
        mFilters.push_back( std::make_unique<StateVariableFilter>() );
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout BassEnvelopeFilterAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "attack",
        "Attack",
        1.0,
        1000.0,
        50.0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "release",
        "Release",
        1.0,
        1000.0,
        50.0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "q_factor",
        "Q",
        0.001,
        20.0,
        0.5));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "depth",
        "Depth",
        0.001,
        1.0,
        0.5));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "drywet",
        "Dry/Wet",
        0.0,
        1.0,
        1.0));

    return layout;
}

BassEnvelopeFilterAudioProcessor::~BassEnvelopeFilterAudioProcessor()
{
}

//==============================================================================
const juce::String BassEnvelopeFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BassEnvelopeFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BassEnvelopeFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BassEnvelopeFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BassEnvelopeFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BassEnvelopeFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BassEnvelopeFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BassEnvelopeFilterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BassEnvelopeFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void BassEnvelopeFilterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BassEnvelopeFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    for (int i = 0; i < getTotalNumInputChannels(); ++i) {
        mEnvelopeFollowers[i]->setParams(*mAttackParameter, *mReleaseParameter, sampleRate);
        mEnvelopeFollowers[i]->reset();
        mFilters[i]->setSampleRate(sampleRate);
        mFilters[i]->reset();
    }
}

void BassEnvelopeFilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool BassEnvelopeFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void BassEnvelopeFilterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        mEnvelopeFollowers[channel]->setAttack(*mAttackParameter);
        mEnvelopeFollowers[channel]->setRelease(*mReleaseParameter);
        mFilters[channel]->setResonance(*mQParameter);

        auto* channelData = buffer.getWritePointer(channel);

        // ..do something to the data...
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            mFilters[channel]->setCutoffFrequency( *mDepthParameter * mEnvelopeFollowers[channel]->getNextValue(channelData[sample]) );
            channelData[sample] = *mDrywetParameter * mFilters[channel]->getNextValue(channelData[sample])
                                  + (1 - *mDrywetParameter) * channelData[sample];
        }
    }

}

//==============================================================================
bool BassEnvelopeFilterAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BassEnvelopeFilterAudioProcessor::createEditor()
{
    // return new BassEnvelopeFilterAudioProcessorEditor (*this);
    return nullptr;
}

//==============================================================================
void BassEnvelopeFilterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BassEnvelopeFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BassEnvelopeFilterAudioProcessor();
}
