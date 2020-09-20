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
                       ),
#endif
    parameters (*this, nullptr, Identifier ("ronn"),
    {
        std::make_unique<AudioParameterInt>   ("layers", "Layers", 1, 12, 6),
        std::make_unique<AudioParameterInt>   ("kernel", "Kernel Width", 1, 32, 3),
        std::make_unique<AudioParameterInt>   ("channels", "Channels", 1, 64, 8),
        std::make_unique<AudioParameterFloat> ("inputGain", "Input Gain", 0.0f, 2.0f, 1.0f),   
        std::make_unique<AudioParameterFloat> ("outputGain", "Output Gain", 0.0f, 2.0f, 1.0f),
        std::make_unique<AudioParameterBool>  ("useBias", "Use Bias", false),
        std::make_unique<AudioParameterInt>   ("activation", "Activation", 1, 10, 1),
        std::make_unique<AudioParameterInt>   ("dilation", "Dilation Factor", 1, 4, 1),
        std::make_unique<AudioParameterInt>   ("initType", "Init Type", 1, 6, 1)
    })
{
 
    layersParameter     = parameters.getRawParameterValue ("layers");
    kernelParameter     = parameters.getRawParameterValue ("kernel");
    channelsParameter   = parameters.getRawParameterValue ("channels");
    inputGainParameter  = parameters.getRawParameterValue ("inputGain");
    outputGainParameter = parameters.getRawParameterValue ("outputGain");
    useBiasParameter    = parameters.getRawParameterValue ("useBias");
    activationParameter = parameters.getRawParameterValue ("activation");
    dilationParameter   = parameters.getRawParameterValue ("dilation");
    initTypeParameter   = parameters.getRawParameterValue ("initType");


    // neural network model
    model = std::make_shared<Model>(nInputs, 
                                   nOutputs, 
                                   *layersParameter, 
                                   *channelsParameter, 
                                   *kernelParameter, 
                                   *dilationParameter,
                                   *useBiasParameter, 
                                   *activationParameter,
                                   *initTypeParameter);
}

RonnAudioProcessor::~RonnAudioProcessor()
{
    // we may need to delete the model here
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
void RonnAudioProcessor::prepareToPlay (double sampleRate_, int samplesPerBlock_)
{
    // store the sample rate for future calculations
    sampleRate = sampleRate_;
    blockSamples = samplesPerBlock_;

    calculateReceptiveField();      // compute the receptive field, make sure it's up to date
    setupBuffers();                 // setup the buffer for handling context
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

void RonnAudioProcessor::calculateReceptiveField()
{
    int k = *kernelParameter;
    int d = *dilationParameter;
    int l = *layersParameter;
    double rf =  k * d;

    for (int layer = 1; layer < l; ++layer) {
        rf = rf + ((k-1) * pow(d,layer));
    }

    receptiveFieldSamples = rf; // store in attribute
    //std::cout << receptiveFieldSamples << std::endl;
}

void RonnAudioProcessor::setupBuffers()
{
    // compute the size of the buffer which will be passed to model
    mBufferLength = (int)(receptiveFieldSamples + blockSamples - 1);
    iBufferLength = (int)(receptiveFieldSamples + blockSamples - 1);

    // Initialize the to 2 channels (stereo)
    // and mBufferLength samples per channel
    mBuffer.setSize(1, mBufferLength);
    mBuffer.clear();
    mBufferReadIdx = 0;
    mBufferWriteIdx = 0;

    // this buffer stories copy of data in mBuffer (always read from index 0)
    iBuffer.setSize(1, iBufferLength);
    iBuffer.clear();
}

void RonnAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto inChannels  = getTotalNumInputChannels();
    auto outChannels = getTotalNumOutputChannels();

    if (modelChange == true) {
        buildModel();
        calculateReceptiveField();
        setupBuffers();
        modelChange = false;
    }

    // get pointer to the input data
    auto* channelData = buffer.getWritePointer(0);

    // first we copy the input to circular buffer
    for (int n = 0; n < blockSamples; n++) {
        mBuffer.setSample(0, mBufferWriteIdx, buffer.getSample(0, n));
        mBufferWriteIdx = mBufferWriteIdx + 1;
        if (mBufferWriteIdx > (mBufferLength-1)){
            mBufferWriteIdx = 0;
        }
    }

    mBufferReadIdx = mBufferWriteIdx; // start reading from right after where we finished writing

    // now we read these data into new time-aligned buffer
    for (int n = 0; n < mBufferLength; n++) {
        iBuffer.setSample(0, n, mBuffer.getSample(0, mBufferReadIdx));
        mBufferReadIdx = mBufferReadIdx + 1; 
        if (mBufferReadIdx > (mBufferLength-1)){
            mBufferReadIdx = 0;
        }
    }

    std::vector<int64_t> sizes = {iBufferLength};
    auto* iBufferData = iBuffer.getWritePointer(0);
    at::Tensor tensorFrame = torch::from_blob(iBufferData, sizes);
    tensorFrame = torch::mul(tensorFrame, inputGainParameter->load());
    tensorFrame = torch::reshape(tensorFrame, {1,1,iBufferLength});
    //std::cout << "buffer" << tensorFrame.sizes() << std::endl;

    //auto paddedFrame = torch::nn::ConstantPad1d(
    //                                torch::nn::ConstantPad1dOptions
    //                                ({padSize, 0}, 0.0))
    //                                (tensorFrame);

    auto outputFrame = model->forward(tensorFrame);     // process audio through network

    //std::cout << "output" << outputFrame.sizes() << std::endl;

    // now load the output channels back into the buffer
    for (int channel = 0; channel < outChannels; ++channel) {
        auto outputData = outputFrame.index({0,channel,torch::indexing::Slice()});      // index the proper output channel
        //outputData = outputData * (outputGainParameter->load());                      // apply the output gain
        auto outputDataPtr = outputData.data_ptr<float>();                              // get pointer to the output data
        buffer.copyFrom(channel,0,outputDataPtr,blockSamples);                          // copy output data to buffer
    }

    //for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //{
    //    input = torch::mul(input, inputGainParameter->load());      // apply input gain 
    //    auto output = model->forward(paddedFrame);                     
    //    output = torch::mul(output, outputGainParameter->load());  // apply the output gain
    //    auto outputData = output.data_ptr<float>();         
    //    buffer.copyFrom(channel,0,outputData,frameSize);    // copy output data to buffer
    // }
}

//==============================================================================
bool RonnAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* RonnAudioProcessor::createEditor()
{
    return new RonnAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void RonnAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void RonnAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
 
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (ValueTree::fromXml (*xmlState));
}

//==============================================================================

void RonnAudioProcessor::buildModel() 
{
    model.reset(new Model(nInputs, 
                        nOutputs, 
                        *layersParameter, 
                        *channelsParameter, 
                        *kernelParameter, 
                        *dilationParameter,
                        *useBiasParameter,
                        *activationParameter,
                        *initTypeParameter));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RonnAudioProcessor();
}
