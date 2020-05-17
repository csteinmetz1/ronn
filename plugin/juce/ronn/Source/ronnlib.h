#ifndef RONNLIB_H
#define RONNLIB_H

#include <torch/torch.h>

struct Model : public torch::nn::Module {

    public:

        enum Activation {LeakyReLU, Tanh, Sigmoid, ReLU, ELU, SELU, GELU, RReLU, Softplus, Softshrink};

        Model(int nInputs, 
              int nOutputs, 
              int nLayers, 
              int nChannels, 
              int kWidth, 
              bool useBias, 
              Activation act,
              std::vector<float> dilations);

        torch::Tensor forward(torch::Tensor);
        void init(std::string initType);
        void buildModel();
        int getOutputSize(int frameSize);

        void setBias(bool newBias){bias = newBias;};
        void setInputs(int newInputs){inputs = newInputs;};
        void setLayers(int newLayers){layers = newLayers;};
        void setOutputs(int newOutputs){outputs = newOutputs;};
        void setChannels(int newChannels){channels = newChannels;};
        void setActivation(Activation newActivation){activation = newActivation;};
        void setDilations(std::vector<float> newDilations){dilations = newDilations;};
        void setKernelWidth(int newKernelWidth){kernelWidth = newKernelWidth;};

        bool getBias(){return bias;};
        int getInputs(){return inputs;};
        int getLayers(){return layers;};
        int getOutputs(){return outputs;};
        int getChannels(){return channels;};
        int getKernelWidth(){return kernelWidth;};
        std::vector<float> getDilations(){return dilations;};
        Activation getActivation(){return activation;};

    private:
        int inputs, outputs, layers, channels, kernelWidth;
        bool bias;
        Activation activation;
        std::vector<float> dilations;
        std::vector<torch::nn::Conv1d> conv;      
        torch::nn::LeakyReLU leakyrelu;
};

#endif