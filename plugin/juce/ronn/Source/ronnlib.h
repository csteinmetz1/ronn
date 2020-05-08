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
              std::vector<float> dilations);

        torch::Tensor forward(torch::Tensor);
        void init(std::string initType);
        void buildModel();
        int getOutputSize(int frameSize);

        void setBias(bool newBias){bias = newBias; buildModel();};
        void setInputs(int newInputs){inputs = newInputs; buildModel();};
        void setLayers(int newLayers){layers = newLayers; buildModel();};
        void setOutputs(int newOutputs){outputs = newOutputs; buildModel();};
        void setChannels(int newChannels){channels = newChannels; buildModel();};
        void setActivation(std::string newActivation){activation = newActivation;};
        void setDilations(std::vector<float> newDilations){dilations = newDilations; buildModel();};
        void setKernelWidth(int newKernelWidth){kernelWidth = newKernelWidth; buildModel();};

        bool getBias(){return bias;};
        int getInputs(){return inputs;};
        int getLayers(){return layers;};
        int getOutputs(){return outputs;};
        int getChannels(){return channels;};
        int getKernelWidth(){return kernelWidth;};
        std::vector<float> getDilations(){return dilations;};
        std::string getActivation(){return activation;};

    private:
        int inputs, outputs, layers, channels, kernelWidth;
        bool bias;
        std::string activation;
        std::vector<float> dilations;
        int inChannels, outChannels;
        std::vector<torch::nn::Conv1d> conv;      
        torch::nn::LeakyReLU leakyrelu;
};

#endif