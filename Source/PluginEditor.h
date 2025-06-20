/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Attachments.h"
#include "FilmstripSlider.h"

#include "LookAndFeel.h"

//==============================================================================
/**
*/
class ParamsAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ParamsAudioProcessorEditor (ParamsAudioProcessor&);
    ~ParamsAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ParamsAudioProcessor& audioProcessor;

	FilmstripSlider delaySlider;
    APVTS::SliderAttachment delayAttachment;
    
    juce::ComboBox timeSignatureBox;
    Better::ComboBoxAttachment timeSignatureAttachment;

	juce::ToggleButton syncBox;
	Better::ButtonAttachment syncAttachment;

	MyLookAndFeel myLookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamsAudioProcessorEditor)
};
