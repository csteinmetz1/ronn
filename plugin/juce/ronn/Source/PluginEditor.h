/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class RonnAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    enum
    {
        paramControlHeight = 40,
        paramLabelWidth    = 80,
        paramSliderWidth   = 300
    };

    RonnAudioProcessorEditor (RonnAudioProcessor&, AudioProcessorValueTreeState&);
    ~RonnAudioProcessorEditor();

    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;


    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RonnAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;

    Slider layersSlider, kernelSlider, channelsSlider;
    std::unique_ptr<SliderAttachment> layersAttachment, kernelAttachment, channelsAttachment;
 
    ToggleButton useBiasButton; 
    std::unique_ptr<ButtonAttachment> useBiasAttachment;

    ComboBox dilationsComboBox, activationsComboBox;
    std::unique_ptr<ComboBoxAttachment> dilationsAttachment, activationsAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RonnAudioProcessorEditor)
};
