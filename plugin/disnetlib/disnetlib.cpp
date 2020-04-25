#include<iostream>
#include<string>
#include <cstdint>
#include<torch/torch.h>

struct Model : torch::nn::Module {
    Model(int nInputs, int nOutputs, int nLayers, int nChannels, int kernelWidth, bool bias) {

        for (int i = 0; i < nLayers; i++) {
            if (i == 0) {
                inChannels = nInputs;
                outChannels = nChannels;
            }
            else if (i + 1 == nLayers) {
                inChannels = nChannels;
                outChannels = nOutputs;
            }
            else {
                inChannels = nChannels;
                outChannels = nChannels;
            }
            conv.push_back(torch::nn::Conv1d(
                torch::nn::Conv1dOptions(inChannels,outChannels,kernelWidth)
                .stride(1)
                .bias(bias)));
        }

        // now register each convolutional layer
        for (auto i = 0; i < conv.size(); i++) {
            register_module("conv"+std::to_string(i), conv[i]);
        }

        // and setup the activation functions
        leakyrelu = torch::nn::LeakyReLU(
                        torch::nn::LeakyReLUOptions()
                        .negative_slope(0.2));
    }
    // the forward operation
    torch::Tensor forward(torch::Tensor x){
        // we iterate over the convolutions
        for (auto i = 0; i < conv.size(); i++) {
            if (i + 1 < conv.size()) {
                x = torch::relu(conv[i](x));
            }
            else {
                x = conv[i](x);
            }
        }
        return x;
    }
    int inChannels, outChannels;
    std::vector<torch::nn::Conv1d> conv;

    // activation functions
    torch::nn::ReLU relu;
    torch::nn::Tanh tanh;             
    torch::nn::LeakyReLU leakyrelu;
    torch::nn::
};

int main(){

    // don't compute gradients
    torch::NoGradGuard no_grad_guard; 

    // define the model config
    int nInputs     = 1;
    int nOutputs    = 2;
    int nLayers     = 6;
    int nChannels   = 8;
    int kernelWidth = 3;
    bool bias       = false;

    // build the model
    Model model(nInputs, nOutputs, nLayers, nChannels, kernelWidth, bias);
    
    // pass through a random tensor
    auto in = torch::rand({1,1,44100});
    auto out = model.forward(in);

    for (auto i = 0; i < out.dim(); i++){
        std::cout << out.size(i) << std::endl;
    }

}