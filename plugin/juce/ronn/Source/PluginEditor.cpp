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
    useBiasButton.setButtonText ("Bias");

    addAndMakeVisible (layersSlider);
    addAndMakeVisible (kernelSlider);
    addAndMakeVisible (channelsSlider);
    addAndMakeVisible (dilationsComboBox);
    addAndMakeVisible (activationsComboBox);
    addAndMakeVisible (useBiasButton);

    // add options to comboboxes
    dilationsComboBox.addItem ("Plain", 1);
    activationsComboBox.addItem("LeakyReLU", 1);
    activationsComboBox.addItem("Tanh", 2);
    activationsComboBox.addItem("Sigmoid", 3);
    activationsComboBox.addItem("ReLU", 4);
    activationsComboBox.addItem("ELU", 5);
    activationsComboBox.addItem("SELU", 6);
    activationsComboBox.addItem("GELU", 7);
    activationsComboBox.addItem("RReLU", 8);
    activationsComboBox.addItem("Softplus", 9);
    activationsComboBox.addItem("Softshrink", 10); 

    layersAttachment.reset      (new SliderAttachment   (valueTreeState, "layers", layersSlider));
    kernelAttachment.reset      (new SliderAttachment   (valueTreeState, "kernel", kernelSlider));
    channelsAttachment.reset    (new SliderAttachment   (valueTreeState, "channels", channelsSlider));
    dilationsAttachment.reset   (new ComboBoxAttachment (valueTreeState, "dilations", dilationsComboBox));
    activationsAttachment.reset (new ComboBoxAttachment (valueTreeState, "activations", activationsComboBox));
    useBiasAttachment.reset     (new ButtonAttachment   (valueTreeState, "useBias", useBiasButton));

    // callbacks
    layersSlider.onValueChange   = [this] { processor.modelChange = true; };
    kernelSlider.onValueChange   = [this] { processor.modelChange = true; };
    channelsSlider.onValueChange = [this] { processor.modelChange = true; };
    dilationsComboBox.onChange   = [this] { processor.modelChange = true; };
    activationsComboBox.onChange = [this] { processor.modelChange = true; };
    useBiasButton.onStateChange  = [this] { processor.modelChange = true; };

    getLookAndFeel().setColour (Slider::thumbColourId, Colours::grey);
    getLookAndFeel().setColour (Slider::trackColourId, Colours::lightgrey);
    getLookAndFeel().setColour (Slider::backgroundColourId, Colours::lightgrey);    
    getLookAndFeel().setColour (Slider::textBoxBackgroundColourId, Colours::white);
    getLookAndFeel().setColour (Slider::textBoxTextColourId, Colours::darkgrey);
    getLookAndFeel().setColour (Slider::textBoxHighlightColourId, Colours::darkgrey);
    getLookAndFeel().setColour (Slider::textBoxOutlineColourId, Colours::darkgrey);

    getLookAndFeel().setColour (ComboBox::backgroundColourId, Colours::white);
    getLookAndFeel().setColour (ComboBox::textColourId, Colours::darkgrey);
    getLookAndFeel().setColour (ComboBox::outlineColourId, Colours::darkgrey);
    getLookAndFeel().setColour (ComboBox::arrowColourId, Colours::darkgrey);

    getLookAndFeel().setColour (PopupMenu::backgroundColourId, Colours::white);
    getLookAndFeel().setColour (PopupMenu::textColourId, Colours::darkgrey);
    getLookAndFeel().setColour (PopupMenu::highlightedBackgroundColourId, Colours::lightgrey);
    getLookAndFeel().setColour (PopupMenu::highlightedTextColourId, Colours::darkgrey);

    setSize (800, 280);
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
    //g.setFont (15.0f);
 
    //g.drawFittedText ("ronn", 0, 0, getWidth(), 30, Justification::centred, 1);
}

void RonnAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();

    auto marginTop          = 32;
    auto contentPadding     = 12;
    auto contentItemHeight  = 24;
    auto scopeWidth         = 280;

    area.removeFromTop(marginTop);
    area.removeFromRight(scopeWidth);

    layersSlider.setBounds        (area.removeFromTop (contentItemHeight));
    area.removeFromTop(contentPadding);
    kernelSlider.setBounds        (area.removeFromTop (contentItemHeight));
    area.removeFromTop(contentPadding);
    channelsSlider.setBounds      (area.removeFromTop (contentItemHeight));
    area.removeFromTop(contentPadding);
    dilationsComboBox.setBounds   (area.removeFromTop (contentItemHeight));
    area.removeFromTop(contentPadding);
    activationsComboBox.setBounds (area.removeFromTop (contentItemHeight));
    area.removeFromTop(contentPadding);
    useBiasButton.setBounds       (area.removeFromTop (contentItemHeight));
    area.removeFromTop(contentPadding);

}

