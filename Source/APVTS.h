#pragma once

#include <JuceHeader.h>
#include "Params.h"

class APVTS : public juce::AudioProcessorValueTreeState
{
public:
	APVTS (juce::AudioProcessor& processor);
	~APVTS() override = default;

	static AudioProcessorValueTreeState::ParameterLayout createParameterLayout(juce::AudioProcessor& processorToUse);
};
