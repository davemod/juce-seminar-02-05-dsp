/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ParamsAudioProcessor::ParamsAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	dryWetMixer.setWetMixProportion (0.2f);
}

ParamsAudioProcessor::~ParamsAudioProcessor()
{
}

//==============================================================================
const juce::String ParamsAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ParamsAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ParamsAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ParamsAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ParamsAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ParamsAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ParamsAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ParamsAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ParamsAudioProcessor::getProgramName (int index)
{
    return {};
}

void ParamsAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ParamsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = static_cast<uint32> (samplesPerBlock);
	spec.numChannels = static_cast<uint32> (juce::jmax (getTotalNumInputChannels (), getTotalNumOutputChannels()));

	delayLine.setMaximumDelayInSamples (static_cast<int> (sampleRate * 5.0)); // optional, get max value from parameter 
    delayLine.prepare (spec);

	dryWetMixer.prepare (spec);
}

void ParamsAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ParamsAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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
#endif

void ParamsAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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

	// 1. ProcessContext, was braucht der ctor
	// 2. AudioBlock, was braucht der ctor

	dsp::AudioBlock<float> audioBlock{ buffer };
	dsp::ProcessContextReplacing<float> context( audioBlock );

	// atomic<float> *, pointer zu einer atomic<float>
	std::atomic<float> * paramValueAtomicPointer = getPluginState ().getRawParameterValue (Params::delayTimeId.getParamID ());
	float delayTimeInSeconds = paramValueAtomicPointer->load () / 1000.0f; // get the delay time in seconds
	float delayTimeInSamples = delayTimeInSeconds * spec.sampleRate; // convert to samples

	dryWetMixer.pushDrySamples (audioBlock);

	delayLine.setDelay (delayTimeInSamples);
	delayLine.process (context);

	dryWetMixer.mixWetSamples (audioBlock);
}

//==============================================================================
bool ParamsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ParamsAudioProcessor::createEditor()
{
    return new ParamsAudioProcessorEditor (*this);
}

//==============================================================================
void ParamsAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
	juce::MemoryOutputStream mos (destData, true);
    getPluginState ().copyState ().writeToStream (mos);
}

void ParamsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    getPluginState ().replaceState (juce::ValueTree::readFromData (data, sizeInBytes));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ParamsAudioProcessor();
}
