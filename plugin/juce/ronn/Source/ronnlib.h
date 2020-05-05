#ifndef RONNLIB_H
#define RONNLIB_H

#include <torch/torch.h>

struct Model : public torch::nn::Module {

    public:
        Model(int nInputs, 
              int nOutputs, 
              int nLayers, 
              int nChannels, 
              int kernelWidth, 
              bool bias, 
              std::string act,
              float* dilations);
        torch::Tensor forward(torch::Tensor);
        void init(std::string initType);
        int getOutputSize(int frameSize);

        void setInputs(int newInputs){inputs = newInputs;};
        void setOutputs(int newOutputs){outputs = newOutputs;};
        void setLayers(int newLayers){layers = newLayers;};
        void setChannels(int newChannels){channels = newChannels;};
        void setKernelWidth(int newKernelWidth){kernelWidth = newKernelWidth;};
        void setBias(bool newBias){bias = bias;};
        void setActivation(std::string newActivation){activation = newActivation;};
        void setDilations(float* newDilations){dilations = newDilations;};

        int getInputs(){return inputs;};
        int getOutputs(){return outputs;};
        int getLayers(){return layers;};
        int getChannels(){return channels;};
        int getKernelWidth(){return kernelWidth;};
        bool getBias(){return bias;};
        std::string getActivation(){return activation;};
        float* getDilations(){return dilations;};

    private:
        int inputs, outputs, layers, channels, kernelWidth;
        bool bias;
        std::string activation;
        float* dilations;
        int inChannels, outChannels;
        std::vector<torch::nn::Conv1d> conv;      
        torch::nn::LeakyReLU leakyrelu;
};

#endif