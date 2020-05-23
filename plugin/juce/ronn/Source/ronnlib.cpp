#include <iostream>
#include <string>
#include <cstring>
#include <cstdint>
#include <torch/torch.h>

#include "ronnlib.h"

Model::Model(int nInputs, 
             int nOutputs, 
             int nLayers, 
             int nChannels, 
             int kWidth, 
             bool useBias, 
             Activation act,
             std::vector<float> cDilations) {

        inputs = nInputs;
        outputs = nOutputs;
        layers = nLayers;
        channels = nChannels;
        kernelWidth = kWidth;
        bias = useBias;
        activation = act;
        dilations = cDilations;

        buildModel();

        // and setup the activation functions
        leakyrelu = torch::nn::LeakyReLU(
                        torch::nn::LeakyReLUOptions()
                        .negative_slope(0.2));
}

void Model::buildModel() {

    int inChannels, outChannels;

    //const float* convDilations = getDilations();

    // construct the convolutional layers
    for (int i = 0; i < getLayers(); i++) {
        if (i == 0 && getLayers() > 1) {
            inChannels = getInputs();
            outChannels = getChannels();
        }
        else if (i == 0) {
            inChannels = getInputs();
            outChannels = getOutputs(); 
        }
        else if (i + 1 == getLayers()) {
            inChannels = getChannels();
            outChannels = getOutputs();
        }
        else {
            inChannels = getChannels();
            outChannels = getChannels();
        }
        conv.push_back(torch::nn::Conv1d(
            torch::nn::Conv1dOptions(inChannels,outChannels,getKernelWidth())
            .stride(1)
            .dilation(getDilations()[i])
            .bias(getBias())));
    }

    // now register each convolutional layer
    for (auto i = 0; i < getLayers(); i++) {
        register_module("conv"+std::to_string(i), conv[i]);
    }
}

// the forward operation
torch::Tensor Model::forward(torch::Tensor x) {
    // we iterate over the convolutions
    for (auto i = 0; i < getLayers(); i++) {
        if (i + 1 < getLayers()) {
            switch (getActivation()) {
                case LeakyReLU:     x = leakyrelu         (conv[i](x)); break;
                case Tanh:          x = torch::tanh       (conv[i](x)); break;
                case Sigmoid:       x = torch::sigmoid    (conv[i](x)); break;
                case ReLU:          x = torch::relu       (conv[i](x)); break;
                case ELU:           x = torch::elu        (conv[i](x)); break;
                case SELU:          x = torch::selu       (conv[i](x)); break;
                case GELU:          x = torch::gelu       (conv[i](x)); break;
                case RReLU:         x = torch::rrelu      (conv[i](x)); break;
                case Softplus:      x = torch::softplus   (conv[i](x)); break;
                case Softshrink:    x = torch::softshrink (conv[i](x)); break;
                default:            x =                   (conv[i](x)); break;
            }
        }
        else
            x = conv[i](x);
    }
    return x;
}

void Model::init(std::string initType){
    for (auto i = 0; i < getLayers(); i++) {
        switch(getInitType())
        {
            case normal:            torch::nn::init::normal_            (conv[i]->weight);
            case uniform:           torch::nn::init::uniform_           (conv[i]->weight, -0.25, 0.25);
            case xavier_normal:     torch::nn::init::xavier_normal_     (conv[i]->weight);
            case xavier_uniform:    torch::nn::init::xavier_uniform_    (conv[i]->weight);
            case kaiming_normal:    torch::nn::init::kaiming_normal_    (conv[i]->weight);
            case kamming_uniform:   torch::nn::init::kaiming_uniform_   (conv[i]->weight);
        }
    }
}

int Model::getOutputSize(int frameSize){
    int outputSize = frameSize;
    for (auto i = 0; i < getLayers(); i++) {
        outputSize = outputSize - ((getKernelWidth()-1) * getDilations()[i]);
    }
    return outputSize;
}
