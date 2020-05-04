#ifndef RONNLIB_H
#define RONNLIB_H

#include <torch/torch.h>

class Model : public torch::nn::Module {

    int inChannels, outChannels;
    int nConv;
    std::vector<torch::nn::Conv1d> conv;

    // activation functions
    std::string activation;         
    torch::nn::LeakyReLU leakyrelu;

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

};

#endif