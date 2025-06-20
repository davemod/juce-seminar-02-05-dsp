#pragma once

#include <JuceHeader.h>

class DelayTimeParameter : public juce::AudioParameterFloat, private juce::Timer
{
public:
	DelayTimeParameter (juce::AudioProcessor& p)
	: juce::AudioParameterFloat (Params::delayTimeId, "Delay Time", { 10.f, 5000.f },100.f)
	, processor (p)
	{
		startTimerHz (10);
	}

	juce::String getLabel () const override
	{
		return get () >= 1000.f ? "s" : "ms";
	}
	
	juce::String getText (float value, int length) const override
	{
		auto range = getNormalisableRange();
		auto denormalised = range.convertFrom0to1(value);
		
		if (denormalised >= 1000.f)
			return juce::String (denormalised, 2);
		
		return juce::String (denormalised, 2);
	}
	
	void valueChanged (float newValue) override
	{
		const auto checkValues = [&]()
		{
			if (newValue >= 1000.f && lastValue < 1000.f)
				return true;
			
			if (lastValue >= 1000.f && newValue < 1000.f)
				return true;
			
			return false;
		};
		
		if (checkValues ())
			needsUpdate.set (true);
		
		lastValue = newValue;
	}
	
private:
	juce::AudioProcessor& processor;
	juce::Atomic<bool> needsUpdate{ false };
	
	juce::String currentLabel;
	float lastValue{ 0.f };
	
	void timerCallback () override
	{
		if (needsUpdate.compareAndSetBool (false, true))
			processor.updateHostDisplay (juce::AudioProcessor::ChangeDetails ().withParameterInfoChanged (true));
	}
};