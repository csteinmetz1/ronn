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

    getLookAndFeel().setColour (Slider::thumbColourId, Colours::grey);
    getLookAndFeel().setColour (Slider::trackColourId, Colours::lightgrey);
    getLookAndFeel().setColour (Slider::backgroundColourId, Colours::lightgrey);    
    getLookAndFeel().setColour (Slider::textBoxBackgroundColourId, Colours::white);
    getLookAndFeel().setColour (Slider::textBoxTextColourId, Colours::darkgrey);
    getLookAndFeel().setColour (Slider::textBoxHighlightColourId, Colours::darkgrey);
    getLookAndFeel().setColour (Slider::textBoxOutlineColourId, Colours::white);
    getLookAndFeel().setColour (Slider::rotarySliderFillColourId, Colours::lightgrey);
    getLookAndFeel().setColour (Slider::rotarySliderOutlineColourId, Colours::lightgrey);

    getLookAndFeel().setColour (ComboBox::backgroundColourId, Colours::white);
    getLookAndFeel().setColour (ComboBox::textColourId, Colours::darkgrey);
    getLookAndFeel().setColour (ComboBox::outlineColourId, Colours::white);
    getLookAndFeel().setColour (ComboBox::arrowColourId, Colours::darkgrey);

    getLookAndFeel().setColour (Label::textColourId, Colours::darkgrey);

    getLookAndFeel().setColour (PopupMenu::backgroundColourId, Colours::white);
    getLookAndFeel().setColour (PopupMenu::textColourId, Colours::darkgrey);
    getLookAndFeel().setColour (PopupMenu::highlightedBackgroundColourId, Colours::lightgrey);
    getLookAndFeel().setColour (PopupMenu::highlightedTextColourId, Colours::darkgrey);

    useBiasButton.setButtonText ("Bias");

    Colour fillColour = Colour (0xffececec); // side panel color

    inputGainSlider.setRange(-24, 24);
    inputGainSlider.setSliderStyle (Slider::Rotary);
    inputGainSlider.setTextBoxStyle (Slider::TextBoxRight, false, 50, 24);//(Slider::NoTextBox, false, 0, 0);
    inputGainSlider.onValueChange = [this] {processor.inputGainLn = juce::Decibels::decibelsToGain((float) inputGainSlider.getValue());};
    inputGainSlider.setValue (juce::Decibels::gainToDecibels(processor.inputGainLn));
    inputGainSlider.setColour (Slider::textBoxBackgroundColourId, fillColour);
    inputGainSlider.setColour (Slider::textBoxOutlineColourId, fillColour);
    inputGainLabel.setText ("in", dontSendNotification);
    inputGainLabel.attachToComponent (&inputGainSlider, true); 
    
    outputGainSlider.setRange(-24, 24);
    outputGainSlider.setSliderStyle (Slider::Rotary);
    outputGainSlider.setTextBoxStyle (Slider::TextBoxRight, false, 50, 24);//(Slider::NoTextBox, false, 0, 0);
    outputGainSlider.onValueChange = [this] {processor.outputGainLn = juce::Decibels::decibelsToGain((float) inputGainSlider.getValue());};
    outputGainSlider.setValue (juce::Decibels::gainToDecibels(processor.outputGainLn));
    outputGainSlider.setColour (Slider::textBoxBackgroundColourId, fillColour);
    outputGainSlider.setColour (Slider::textBoxOutlineColourId, fillColour);
    outputGainLabel.setText ("out", dontSendNotification);
    outputGainLabel.attachToComponent (&outputGainSlider, true); 

    layersSlider.setTextBoxStyle (Slider::TextBoxRight, false, 30, 24);
    kernelSlider.setTextBoxStyle (Slider::TextBoxRight, false, 30, 24);
    channelsSlider.setTextBoxStyle (Slider::TextBoxRight, false, 30, 24);

    addAndMakeVisible (layersSlider);
    addAndMakeVisible (kernelSlider);
    addAndMakeVisible (channelsSlider);
    addAndMakeVisible (inputGainSlider);
    addAndMakeVisible (outputGainSlider);
    addAndMakeVisible (dilationsComboBox);
    addAndMakeVisible (activationsComboBox);
    addAndMakeVisible (initTypeComboBox);
    addAndMakeVisible (useBiasButton);
    addAndMakeVisible (inputGainLabel);
    addAndMakeVisible (outputGainLabel);

    // attach labels
    addAndMakeVisible (layersLabel);
    layersLabel.setText ("layers", dontSendNotification);
    layersLabel.attachToComponent (&layersSlider, true); 
    addAndMakeVisible (kernelLabel);
    kernelLabel.setText ("kernel", dontSendNotification);
    kernelLabel.attachToComponent (&kernelSlider, true); 
    addAndMakeVisible (channelsLabel);
    channelsLabel.setText ("channels", dontSendNotification);
    channelsLabel.attachToComponent (&channelsSlider, true); 

    // add options to comboboxes
    dilationsComboBox.addItem ("1^n", 1);
    dilationsComboBox.addItem ("2^n", 2);
    dilationsComboBox.addItem ("3^n", 3);
    dilationsComboBox.addItem ("4^n", 4);

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

    initTypeComboBox.addItem("Normal", 1);
    initTypeComboBox.addItem("Uniform", 2);
    initTypeComboBox.addItem("Xavier (Normal)", 3);
    initTypeComboBox.addItem("Xavier (Uniform)", 4);
    initTypeComboBox.addItem("Kaiming (Normal)", 5);
    initTypeComboBox.addItem("Kaiming (Uniform)", 6);

    receptiveFieldTextEditor.setColour (TextEditor::backgroundColourId, fillColour);
    receptiveFieldTextEditor.setColour (TextEditor::outlineColourId, fillColour);
    receptiveFieldTextEditor.setColour (TextEditor::textColourId, Colours::darkgrey);
    receptiveFieldTextEditor.setColour (TextEditor::highlightColourId, Colours::darkgrey);
    receptiveFieldTextEditor.setReadOnly(true);
    receptiveFieldTextEditor.setFont(Font (15.0f));
    receptiveFieldTextEditor.setText("0", false);
    addAndMakeVisible(receptiveFieldTextEditor);

    seedTextEditor.setColour (TextEditor::backgroundColourId, fillColour);
    seedTextEditor.setColour (TextEditor::outlineColourId, fillColour);
    seedTextEditor.setColour (TextEditor::textColourId, Colours::darkgrey);
    seedTextEditor.setColour (TextEditor::highlightColourId, Colours::darkgrey);
    seedTextEditor.setReadOnly(false);
    seedTextEditor.setFont(Font (15.0f));
    seedTextEditor.setText("0", false);
    addAndMakeVisible(seedTextEditor);

    layersAttachment.reset      (new SliderAttachment   (valueTreeState, "layers", layersSlider));
    kernelAttachment.reset      (new SliderAttachment   (valueTreeState, "kernel", kernelSlider));
    channelsAttachment.reset    (new SliderAttachment   (valueTreeState, "channels", channelsSlider));
    inputGainAttachment.reset   (new SliderAttachment   (valueTreeState, "inputGain", inputGainSlider));
    outputGainAttachment.reset  (new SliderAttachment   (valueTreeState, "outputGain", outputGainSlider));
    dilationsAttachment.reset   (new ComboBoxAttachment (valueTreeState, "dilation", dilationsComboBox));
    activationsAttachment.reset (new ComboBoxAttachment (valueTreeState, "activation", activationsComboBox));
    initTypeAttachment.reset    (new ComboBoxAttachment (valueTreeState, "initType", initTypeComboBox));
    useBiasAttachment.reset     (new ButtonAttachment   (valueTreeState, "useBias", useBiasButton));
    //seedAttachment.reset        (new TextBoxAttachment  (valueTreeState, "seed", seedTextEditor));

    // callbacks
    layersSlider.onValueChange   = [this] { updateModelState(); };
    kernelSlider.onValueChange   = [this] { updateModelState(); };
    channelsSlider.onValueChange = [this] { updateModelState(); };
    dilationsComboBox.onChange   = [this] { updateModelState(); };
    activationsComboBox.onChange = [this] { updateModelState(); };
    initTypeComboBox.onChange    = [this] { updateModelState(); };
    useBiasButton.onStateChange  = [this] { updateModelState(); };

    setSize (800, 280);
}

RonnAudioProcessorEditor::~RonnAudioProcessorEditor()
{
}

//==============================================================================
void RonnAudioProcessorEditor::updateModelState(){
  processor.modelChange = true;
  processor.calculateReceptiveField();
  float rfms = (processor.receptiveFieldSamples / processor.sampleRate) * 1000;
  receptiveFieldTextEditor.setText(String(rfms));
}

//==============================================================================
void RonnAudioProcessorEditor::paint (Graphics& g)
{
    // fill the whole window white
    g.fillAll (Colours::white);

    // set the font size and draw text to the screen
    g.setFont (15.0f);

    // fill the right panel with grey
    {
      int x = 600, y = 0, width = 300, height = 280;
      Colour fillColour = Colour (0xffececec);
      g.setColour (fillColour);
      g.fillRect (x, y, width, height);

      g.setColour (Colours::grey);
      g.setFont (Font ("Source Sans Variable", 32.0f, Font::plain).withTypefaceStyle ("Light")); //.withExtraKerningFactor (0.147f));
      g.drawText ("ronn", 550, 0, width, 70, Justification::centred, true);
      g.setFont (Font ("Source Sans Variable", 10.0f, Font::plain).withTypefaceStyle ("Light")); //.withExtraKerningFactor (0.147f));
      g.drawText ("randomised overdrive", 550, 0, width, 105, Justification::centred, true);
      g.drawText ("neural network", 550, 0, width, 120, Justification::centred, true);
    }
}

void RonnAudioProcessorEditor::resized()
{

    auto marginTop          = 32;
    auto contentPadding     = 12;
    auto sectionPadding     = 18;
    auto contentItemHeight  = 24;
    auto rotaryItemHeight   = 55;
    auto scopeWidth         = 280;
    auto sidePanelWidth     = 200;

    // side panel
    auto area = getLocalBounds();

    area.removeFromTop(marginTop + 40);
    area.removeFromLeft(800 - sidePanelWidth);
    area.removeFromLeft(sectionPadding+10);
    area.removeFromRight(sectionPadding+10);

    inputGainSlider.setBounds  (area.removeFromTop (rotaryItemHeight));
    area.removeFromTop(6);
    outputGainSlider.setBounds (area.removeFromTop (rotaryItemHeight));
    area.removeFromTop(6);
    receptiveFieldTextEditor.setBounds (area.removeFromTop (contentItemHeight));
    area.removeFromTop(3);
    seedTextEditor.setBounds (area.removeFromTop (contentItemHeight));
    area.removeFromTop(3);

    // center panel
    area = getLocalBounds();

    area.removeFromTop(marginTop);
    area.removeFromLeft(scopeWidth);
    area.removeFromLeft(sectionPadding);
    area.removeFromRight(sidePanelWidth);
    area.removeFromRight(sectionPadding);

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
    initTypeComboBox.setBounds    (area.removeFromTop (contentItemHeight));
    area.removeFromTop(contentPadding);
    useBiasButton.setBounds       (area.removeFromTop (contentItemHeight));
    area.removeFromTop(contentPadding);

}

