#include<iostream>
#include<torch/torch.h>


struct Model : torch::nn::Module {
    Model(int nInputs, int nOutputs, int nChannels, int kernelWidth, bool bias) :
        conv1(torch::nn::Conv1dOptions(nInputs,   nChannels, kernelWidth).stride(1).bias(bias)),
        conv2(torch::nn::Conv1dOptions(nChannels, nChannels, kernelWidth).stride(1).bias(bias)),
        conv3(torch::nn::Conv1dOptions(nChannels, nChannels, kernelWidth).stride(1).bias(bias)),
        conv4(torch::nn::Conv1dOptions(nChannels, nChannels, kernelWidth).stride(1).bias(bias)),
        conv5(torch::nn::Conv1dOptions(nChannels, nChannels, kernelWidth).stride(1).bias(bias)),
        conv6(torch::nn::Conv1dOptions(nChannels, nOutputs,  kernelWidth).stride(1).bias(bias)),
        activation(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.2)))
    {
        // construct and register your layers
        register_module("conv1", conv1);
        register_module("conv2", conv2);
        register_module("conv3", conv3);
        register_module("conv4", conv4);
        register_module("conv5", conv5);
        register_module("conv6", conv6);
        register_module("activation", activation);
    }
    // the forward operation (how data will flow from layer to layer)
    torch::Tensor forward(torch::Tensor x){
        // let's pass relu 
        x = activation(conv1(x));
        x = activation(conv2(x));
        x = activation(conv3(x));
        x = activation(conv4(x));
        x = activation(conv5(x));
        x = conv6(x);

        return x;
    }
    torch::nn::Conv1d conv1,conv2,conv3,conv4,conv5,conv6;
    torch::nn::LeakyReLU activation;
};

int main(){

    torch::NoGradGuard no_grad_guard; // don't compute gradients

    Model model(1, 2, 8, 3, false);
    
    auto in = torch::rand({1,1,44100});
    auto out = model.forward(in);

    std::cout << in << std::endl;
    std::cout << out << std::endl;

}