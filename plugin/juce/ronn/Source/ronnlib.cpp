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
             std::string act,
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

    // conv.clear(); // remove previous layers

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
            .dilation(1) //getDilations()[i]
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
            if (activation.compare("ReLU") == 0)
                x = torch::relu(conv[i](x));
            else if (activation.compare("LeakyReLU") == 0)
                x = leakyrelu(conv[i](x));
            else if (activation.compare("Tanh") == 0)
                x = torch::tanh(conv[i](x));
            else if (activation.compare("Sigmoid") == 0)
                x = torch::sigmoid(conv[i](x));
            else if (activation.compare("ELU") == 0)
                x = torch::elu(conv[i](x));
            else if (activation.compare("SELU") == 0)
                x = torch::selu(conv[i](x));
            else if (activation.compare("GELU") == 0)
                x = torch::gelu(conv[i](x));
            else if (activation.compare("RReLU") == 0)
                x = torch::rrelu(conv[i](x));
            else if (activation.compare("Softplus") == 0)
                x = torch::softplus(conv[i](x));
            else if (activation.compare("Softshrink") == 0)
                x = torch::softshrink(conv[i](x));
            else
                x = conv[i](x);
        }
        else {
            x = conv[i](x);
        }
    }
    return x;
}

void Model::init(std::string initType){
    for (auto i = 0; i < getLayers(); i++) {
        if (initType.compare("normal"))
            torch::nn::init::normal_(conv[i]->weight);
        else if (initType.compare("uniform"))
            torch::nn::init::normal_(conv[i]->weight, -0.25, 0.25);
        else if (initType.compare("xavier_normal"))
            torch::nn::init::xavier_normal_(conv[i]->weight);
        else if (initType.compare("xavier_uniform"))
            torch::nn::init::xavier_uniform_(conv[i]->weight);
        else if (initType.compare("kaiming_normal"))
            torch::nn::init::kaiming_normal_(conv[i]->weight);
        else if (initType.compare("kamming_uniform"))
            torch::nn::init::kaiming_uniform_(conv[i]->weight);            
    }
}

int Model::getOutputSize(int frameSize){
    int outputSize = frameSize;
    for (auto i = 0; i < getLayers(); i++) {
        outputSize = outputSize - ((getKernelWidth()-1) * getDilations()[i]);
    }
    return outputSize;
}
