#pragma once

#include <JuceHeader.h>

class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:
	MyLookAndFeel()
	{
		// setting custom default colours in the look and feel
		setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
		setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
		setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkcyan);
		setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey);
	}

	~MyLookAndFeel() override = default;
	
	// overriding single methods to customize the look and feel of components. available methods 
	// you'll find in most juce components, such as Slider, ComboBox, Button, etc., in a struct 
	// called LookAndFeelMethods.
	juce::Path getTickShape (float height) override
	{
		juce::Path path;
		path.addEllipse ({height, height});

		return path;
	}


	// override drawComboBox
	void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
	                  int buttonX, int buttonY, int buttonWidth, int buttonHeight,
	                  juce::ComboBox& box) override
	{
		auto fillColour = box.findColour(juce::ComboBox::backgroundColourId);
		auto borderColour = box.findColour(juce::ComboBox::outlineColourId);

		if (isButtonDown)
		{
			fillColour = fillColour.darker ();
			borderColour = borderColour.darker ();
		}

		g.fillAll(fillColour);
		
		g.setColour(borderColour);
		g.drawRoundedRectangle (0.f, 0.f, (float)width, (float)height, 4.f, 2.f);

		// copied from juce::LookAndFeel_V4::drawComboBox
		juce::Rectangle<int> arrowZone (width - 30, 0, 20, height);
		juce::Path path;
		path.startNewSubPath ((float) arrowZone.getX() + 3.0f, (float) arrowZone.getCentreY() - 2.0f);
		path.lineTo ((float) arrowZone.getCentreX(), (float) arrowZone.getCentreY() + 3.0f);
		path.lineTo ((float) arrowZone.getRight() - 3.0f, (float) arrowZone.getCentreY() - 2.0f);

		g.setColour (box.findColour (juce::ComboBox::arrowColourId).withAlpha ((box.isEnabled() ? 0.9f : 0.2f)));
		g.strokePath (path, juce::PathStrokeType (2.0f));
	}
};