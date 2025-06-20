/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ParamsAudioProcessorEditor::ParamsAudioProcessorEditor (ParamsAudioProcessor& p)
: AudioProcessorEditor (&p)
, audioProcessor (p)
, delayAttachment (p.getPluginState (), Params::delayTimeId.getParamID (), delaySlider)
, timeSignatureAttachment (p.getPluginState (), Params::delayTimeSignatureId.getParamID (), timeSignatureBox)
, syncAttachment (p.getPluginState (), Params::delaySyncId.getParamID (), syncBox)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    addAndMakeVisible(delaySlider);
	addAndMakeVisible(timeSignatureBox);
	addAndMakeVisible(syncBox);

	// 1. Setting style directly on the object, using member functions and colours.
	delaySlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
	delaySlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
	delaySlider.setColour (juce::Slider::textBoxTextColourId, juce::Colours::white);
	delaySlider.setColour (juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
	delaySlider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::darkcyan);
	delaySlider.setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey);

	// 2. Adjusting the style by using custom look and feels. See LookAndFeel.h for details. a
	// look and feel will be valid for all child components, that do not have their own look 
	// and feel set.
	setLookAndFeel (&myLookAndFeel);
}

ParamsAudioProcessorEditor::~ParamsAudioProcessorEditor()
{
	setLookAndFeel (nullptr); // Reset the look and feel to default
	// This will also remove the custom look and feel from the delaySlider, timeSignatureBox, and syncBox.
}

//==============================================================================
void ParamsAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void ParamsAudioProcessorEditor::resized()
{
    auto size = juce::jmin(getWidth(), getHeight());
	auto bounds = getLocalBounds ().withSizeKeepingCentre (size, size);

	syncBox.setBounds (bounds.removeFromTop (40).reduced (6));
	timeSignatureBox.setBounds (bounds.removeFromTop (40).reduced (6));
	delaySlider.setBounds (bounds.reduced (6));
}
