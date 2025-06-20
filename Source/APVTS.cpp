#include "APVTS.h"
#include "DelayTimeParameter.h"

APVTS::APVTS(juce::AudioProcessor &processor)
:
juce::AudioProcessorValueTreeState (processor, nullptr, "STATE", createParameterLayout(processor))
{
}

APVTS::ParameterLayout APVTS::createParameterLayout(juce::AudioProcessor &processorToUse)
{
	AudioProcessorValueTreeState::ParameterLayout layout;
	
	layout.add (std::make_unique<DelayTimeParameter> (processorToUse));
	layout.add (std::make_unique<juce::AudioParameterChoice> (Params::delayTimeSignatureId, "Time Signature", juce::StringArray { "1/32", "1/16", "1/8", "1/4", "1/2", "1" }, 2));
	layout.add (std::make_unique<juce::AudioParameterBool> (Params::delaySyncId, "Sync", false, "", 
		[] (bool value, int) { return value ? "Sync" : "Free"; },
		[] (const juce::String& text) { return text == "Sync"; }
	));

	return layout;
}