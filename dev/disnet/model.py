import torch 

from .utils import get_activation, center_crop

class DISNETModel(torch.nn.Module):
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
        super(DISNETModel, self).__init__()
        
        # if no dilations provided, set them all to 1
        if len(dilations) == 0: dilations = [1 for n in range(n_layers)]

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

        # create each block (layer) in the network
        self.blocks = torch.nn.ModuleList()
        for n in range(n_layers):

            # determeine the input/output channels
            in_channels = n_channels if n != 0 else n_inputs
            out_channels = n_channels if n != (n_layers-1) else n_outputs

            # contruct the components of the block
            block = torch.nn.Sequential(
                torch.nn.Conv1d(in_channels, 
                                out_channels, 
                                kernel_size, 
                                stride=stride, 
                                dilation=dilations[n]),
                get_activation(activation))

            self.blocks.append(block)

        # now apply the weight init to each layer
        for k, param in dict(self.named_parameters()).items():
            if "weight" in k:
                torch.nn.init.normal_(param)                   # smooth 
                #torch.nn.init.uniform_(param,a=-0.1,b=0.1)     # harsh
                #torch.nn.init.constant_(param, 0.1)            # doesn't work
                #torch.nn.init.dirac_(param)                    # nice, but only left channel
                #torch.nn.init.xavier_uniform_(param)           # nice and smooth, even roomy
                #torch.nn.init.xavier_normal_(param)            # similar to uniform, harsher
                #torch.nn.init.kaiming_uniform_(param)          # hmm could be nice 
                #torch.nn.init.orthogonal_(param)               # inconsistent results

    def forward(self, x):

        for block in self.blocks:
            x_in = x        # store for residual
            x = block(x)    # process through convolutional block

            if self.residual:
                x = torch.add(x, center_crop(x_in, x.shape[-1]))

        return x
