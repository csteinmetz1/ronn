#include<iostream>
#include<string>
#include<cstring>
#include<cstdint>
#include<torch/torch.h>

struct Model : torch::nn::Module {
    Model(int nInputs, 
          int nOutputs, 
          int nLayers, 
          int nChannels, 
          int kernelWidth, 
          bool bias, 
          std::string act,
          float* dilations) {

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
                .dilation(dilations[i])
                .bias(bias)));
        }

        activation = act; // set the activation function

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
    torch::Tensor forward(torch::Tensor x) {
        // we iterate over the convolutions
        for (auto i = 0; i < conv.size(); i++) {
            if (i + 1 < conv.size()) {
                if (activation.compare("ReLU") == 0)
                    x = torch::relu(conv[i](x));
                else if (activation.compare("LeakyReLU") == 0)
                    x = leakyrelu(conv[i](x));
                else if (activation.compare("Tanh") == 0)
                    x = torch::tanh(conv[i](x));
                else if (activation.compare("Sigmoid") == 0)
                    x = torch::tanh(conv[i](x));
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

    void init(std::string initType){
        for (auto i = 0; i < conv.size(); i++) {
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

    int inChannels, outChannels;
    std::vector<torch::nn::Conv1d> conv;

    // activation functions
    std::string activation;         
    torch::nn::LeakyReLU leakyrelu;
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
    std::string act = "Softshrink";
    float dilations [6] = {1,2,4,8,16,32};

    // build the model
    Model model(nInputs, 
                nOutputs, 
                nLayers, 
                nChannels, 
                kernelWidth, 
                bias, 
                act,
                dilations);
    
    // pass through a random tensor
    auto in = torch::rand({1,1,44100});
    auto out = model.forward(in);

    for (auto i = 0; i < out.dim(); i++){
        std::cout << out.size(i) << std::endl;
    }

    model.init("xavier_normal");

}