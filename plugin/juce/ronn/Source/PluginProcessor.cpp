/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ronnlib.h"

//==============================================================================
RonnAudioProcessor::RonnAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{

    // parameters
    addParameter (layers = new AudioParameterInt("layers", "Layers", 1, 12, 6));

    // neural network model
    model = std::make_shared<Model>(nInputs, 
                                   nOutputs, 
                                   *layers, 
                                   nChannels, 
                                   kWidth, 
                                   useBias, 
                                   act,
                                   dilations);
}

RonnAudioProcessor::~RonnAudioProcessor()
{
}

//==============================================================================
const String RonnAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RonnAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RonnAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RonnAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RonnAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RonnAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RonnAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RonnAudioProcessor::setCurrentProgram (int index)
{
}

const String RonnAudioProcessor::getProgramName (int index)
{
    return {};
}

void RonnAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void RonnAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // define the model itself


}

void RonnAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RonnAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void RonnAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    int outputSize, padSize, frameSize;
    torch::Tensor input;
    torch::Tensor output;

    frameSize = buffer.getNumSamples();
    outputSize = model->getOutputSize(frameSize);
    padSize = frameSize - outputSize;

    // clear
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // process the audio
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        //auto* channelData = buffer.getWritePointer (channel);
        input = torch::rand({1,1,frameSize + padSize});
        output = model->forward(input);
        auto outputData = output.data_ptr<float>();
        buffer.copyFrom(channel,0,outputData,frameSize);
    }
}

//==============================================================================
bool RonnAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* RonnAudioProcessor::createEditor()
{
    return new RonnAudioProcessorEditor (*this);
}

//==============================================================================
void RonnAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    std::unique_ptr<XmlElement> xml (new XmlElement ("ronn"));
    xml->setAttribute ("layers", (int) *layers);
    copyXmlToBinary (*xml, destData);
}

void RonnAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
 
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName ("ronn"))
            *layers = xmlState->getIntAttribute ("layers", 6);
}

//==============================================================================

void RonnAudioProcessor::buildModel() 
{
    model.reset(new Model(nInputs, 
                                   nOutputs, 
                                   *layers, 
                                   nChannels, 
                                   kWidth, 
                                   useBias, 
                                   act,
                                   dilations));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RonnAudioProcessor();
}
