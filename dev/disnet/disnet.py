import os 
import sys
import torch

from .model import DISNETModel

class DISNET():
    """ Top-level DISNET object. 

    Args:
        n_layers (int, optional): number of convolutional layers in the network.
        kernel_size (int, optional): width of the convoltuional filters. 
        stide (int, optional): stride size for the filters. 
        activation (str, optional): activation type to be applied ['ReLU', 'LeakyReLU', 'Tanh']
        dilations (list, optional): list defining the dilation for each layer (must have same length as `n_layers`).
        init (str, optional): weight intialization method to use (both at init and during randomization).
    """
    def __init__(self, 
                n_inputs=1,
                n_outputs=1,
                n_layers=1, 
                n_channels=1,
                kernel_size=3, 
                stride=1, 
                activation="ReLU", 
                dilations=[], 
                residual=False,
                init=None): 

        self.n_inputs = n_inputs
        self.n_outputs = n_outputs
        self.n_layers = n_layers
        self.n_channels = n_channels
        self.kernel_size = kernel_size
        self.stride = stride
        self.activation = activation
        self.dilations = dilations
        self.residual = residual
        self.init = init

        self.build() # construct the model

    def build(self):
        """ Given the current model configuration, build a PyTorch model. """
        self.model = DISNETModel(n_inputs=self.n_inputs,
                                 n_outputs=self.n_outputs,
                                 n_layers=self.n_layers, 
                                 n_channels=self.n_channels,
                                 kernel_size=self.kernel_size, 
                                 stride=self.stride, 
                                 activation=self.activation,
                                 dilations=self.dilations,
                                 residual=self.residual,
                                 init=self.init)

    def process(self, x):
        """ Given a frame of audio, pass it through the model. """
        ch,s = x.size()

        # pass through the model after adding batch dim
        with torch.no_grad():
            x = self.model(x.view(1,ch,s))

        # return without the batch dimension
        return torch.squeeze(x)

    def randomize(self):
        """ Using the set weight initilization scheme, re-initialization the model. """
        pass