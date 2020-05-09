/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RonnAudioProcessorEditor::RonnAudioProcessorEditor (RonnAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    // these define the parameters of our slider object
    layersSlider.setSliderStyle (Slider::LinearBarVertical);
    layersSlider.setRange(0, 12, 1);
    layersSlider.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    layersSlider.setPopupDisplayEnabled (true, false, this);
    layersSlider.setTextValueSuffix ("");
    layersSlider.setValue(6);
    layersSlider.addListener (this);
 
    // this function adds the slider to the editor
    addAndMakeVisible (&layersSlider);
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
    // sets the position and size of the slider with arguments (x, y, width, height)
    layersSlider.setBounds (40, 30, 20, getHeight() - 60);
}

void RonnAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    if (slider == &layersSlider) {
        std::cout << layersSlider.getValue() << std::endl;
        *processor.layers = layersSlider.getValue();
        processor.buildModel();
        std::cout << "rebuild model" << std::endl;

    }
}
