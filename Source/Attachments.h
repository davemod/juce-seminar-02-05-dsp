#pragma once

#include <JuceHeader.h>

namespace Better
{
	// this class is used to populate a combobox with the choices from the parameter
	class ComboBoxAttachment : public juce::AudioProcessorValueTreeState::ComboBoxAttachment
	{
	public:
		ComboBoxAttachment(juce::AudioProcessorValueTreeState& stateToUse, const juce::String& parameterID, juce::ComboBox& comboBox)
			: juce::AudioProcessorValueTreeState::ComboBoxAttachment(stateToUse, parameterID, comboBox)
		{
			if (auto* parameter = stateToUse.getParameter (parameterID))
			{
				for (int i = 0; i < parameter->getNumSteps (); ++i)
				{
					auto value = parameter->convertTo0to1 (static_cast<float>(i));
					comboBox.addItem (parameter->getText (value, 25), i + 1);
				}

				comboBox.setSelectedItemIndex (parameter->getValue () * (parameter->getNumSteps () - 1), juce::dontSendNotification);
			}
		}
	};


	// class to attach a button to a parameter and use the parameters text as button text
	class ButtonAttachment : juce::Button::Listener, juce::AsyncUpdater
	{
	public:
		ButtonAttachment(juce::AudioProcessorValueTreeState& stateToUse, const juce::String& parameterID, juce::Button& button)
		: state (stateToUse)
		, button (button)
		, parameterID (parameterID)
		{	
			if (auto parameter = state.getParameter (parameterID))
			{
				attachment = std::make_unique<juce::ParameterAttachment> (*parameter, [this] (float f) { setValue (f); }, stateToUse.undoManager);
				attachment->sendInitialUpdate ();
			}
			else
			{
				jassertfalse; // parameter not found
			}
			
			button.setClickingTogglesState (true);
			button.addListener (this);
		}

		~ButtonAttachment() override
		{
			button.removeListener (this);
			attachment.reset();
		}

	private:
		juce::AudioProcessorValueTreeState& state;
		juce::Button& button;
		std::unique_ptr<juce::ParameterAttachment> attachment;

		const juce::String parameterID;

		void buttonClicked (juce::Button * b) override
		{
			if (auto parameter = state.getParameter (parameterID))
				parameter->setValueNotifyingHost (b->getToggleState() ? 1.0f : 0.0f);

			triggerAsyncUpdate ();
		}

		void setValue (float newValue)
		{
			button.setToggleState (newValue >= 0.5f, juce::dontSendNotification);
			triggerAsyncUpdate ();
		}

		void handleAsyncUpdate () override
		{
			if (auto parameter = state.getParameter (parameterID))
				button.setButtonText (parameter->getText (parameter->getValue (), 25));
		}
	};
	
}