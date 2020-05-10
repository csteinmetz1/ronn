/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ronnlib.h"

//==============================================================================
/**
*/
class RonnAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    RonnAudioProcessor();
    ~RonnAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    AudioParameterInt* layers;

    //==============================================================================
    void buildModel();

    // define the model config
    int nInputs     = 1;
    int nOutputs    = 1;
    int nChannels   = 8;
    int kWidth      = 3;
    bool useBias    = false;
    std::string act = "Softplus";
    std::vector<float> dilations {1,2,4,8,16,32,64,128,256,512,1024,2048};
    std::shared_ptr<Model> model;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RonnAudioProcessor)

    //==============================================================================
    AudioProcessorValueTreeState parameters;

};
