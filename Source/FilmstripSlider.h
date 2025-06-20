#pragma once

#include <JuceHeader.h>

class FilmstripSlider : public juce::Slider
{
public:
	FilmstripSlider ()
	{
		filmstrip = juce::ImageFileFormat::loadFrom (BinaryData::knob1_png, BinaryData::knob1_pngSize);
				 // juce::ImageCache::getFromMemory (BinaryData::knob1_png, BinaryData::knob1_pngSize);
	}

	void paint (juce::Graphics& g) override
	{
		// 1. slider value kriegen
		// 2. bild index bestimmen index: 0 = min index: 127 (?) = max
		// 3. clipped image zeichnen // filmstrip.getClippedImage (...)

		// bool isHorizontal{ false };
		// auto index = 
		// auto clippedImage = filmstrip.getClippedImage (juce::Rectangle<int> (0, 0, filmstrip.getWidth (), filmstrip.getHeight ()));
		// g.drawImage (clippedImage, getLocalBounds ().toFloat (), juce::RectanglePlacement::centred);

		// juce::Slider::SliderLayout layout = getLookAndFeel ().getSliderLayout ();
		// layout.sliderBounds;
		// layout.textBoxBounds;

		juce::Slider::paint (g);
	}

private:
	juce::Image filmstrip;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilmstripSlider)
};