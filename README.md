<div  align="center">
<!-- <img src="docs/"> -->

# RONN
Randomised Overdrive Neural Network

[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)

</div>

## What is RONN?
Throughout audio technology history, engineers, circuit designers, and guitarists have searched tirelessly for 
novel, extreme, and exciting effects as a result of clipping audio signals. Whether it be vacuum tubes (valves), 
diodes, transistors, op-amps, microchips, or broken speaker drivers doing the distorting, it seems that we have tried them all. 
But maybe there is at least one area left relatively under-explored, and thats the realm of *neural networks*. 

Now neural networks (simply a series of matrix operations), have been a round for a [bit](). They have actually
ALREADY been used to model distortion and overdrive effects from guitar amplifier and pedals quite a bit 
(such as [here](), [here](), [here](), and [here]()). So then you may be asking, "well how is this any different?"
And the answer is, **RONN** doesn't model ANY pre-existing audio circuit, we don't even bother to train anything! 
Instead we treat the concept of the neural network as a system which can distort a signal, and then we give the user 
control over that system to explore new effects. Get your hands dirty building neural networks without even
touching TensorFlow or PyTorch. 

## Features

### Convolutional Network
- Number of layers
- Kernel size 
- Stride
- Activation functions (ReLU, Tanh, PReLU, etc.)
- Dilation pattern
- Initialization method 
- Randomize all weights
- Transposed convolutions for upsampling

### Further ideas
- a GUI that shows the construction of the network as you add layers, change activations, etc. 
- a transfer function window that shows the shape of the current activation function being used. 
- use FiLM as a further way to adjust the distortion character. For example, create a 2D plane the
  user will sample from and then use a set of linear layers to project this to gamma and beta coefficients
  for each layers (using linear adaptors), then the user can control the input to this MLP. 
- Minimalist UI with white background inspired by Pure Data
- What if you randomised the weights in each layer every time you accessed that layer?

## Setup

To install libtorch (PyTorch C++ do the following).

1. Navigate the to [PyTorch website]() and download the correct `.zip` for your OS.     
2. Unzip this and place the `libtorch` directory into the `plugin` directory. 
3. Run the `build.sh` script, which will build then run the demo program. 