/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RonnAudioProcessorEditor::RonnAudioProcessorEditor (RonnAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    gainLabel.setText ("Gain", dontSendNotification);
    invertButton.setButtonText ("Invert Phase");

    addAndMakeVisible (gainLabel);
    addAndMakeVisible (gainSlider);
    addAndMakeVisible (layersSlider);
    addAndMakeVisible (invertButton);

    gainAttachment.reset   (new SliderAttachment (valueTreeState, "gain", gainSlider));
    layersAttachment.reset (new SliderAttachment (valueTreeState, "layers", layersSlider));
    invertAttachment.reset (new ButtonAttachment (valueTreeState, "invertPhase", invertButton));

    // callbacks
    layersSlider.onValueChange = [this] { processor.modelChange = true; };

    setSize (paramSliderWidth + paramLabelWidth, jmax (100, paramControlHeight * 2));
}

RonnAudioProcessorEditor::~RonnAudioProcessorEditor()
{
}

//==============================================================================
void RonnAudioProcessorEditor::paint (Graphics& g)
{
    // fill the whole window white
    g.fillAll (Colours::white);
 
    // set the current drawing colour to black
    g.setColour (Colours::black);
 
    // set the font size and draw text to the screen
    g.setFont (15.0f);
 
    g.drawFittedText ("ronn", 0, 0, getWidth(), 30, Justification::centred, 1);
}

void RonnAudioProcessorEditor::resized()
{
    auto r = getLocalBounds();

    auto gainRect = r.removeFromTop (paramControlHeight);
    gainLabel .setBounds (gainRect.removeFromLeft (paramLabelWidth));
    layersSlider.setBounds (gainRect);

    invertButton.setBounds (r.removeFromTop (paramControlHeight));
}

