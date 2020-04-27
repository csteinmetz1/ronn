import torch 

from .utils import get_activation, center_crop
from .condition import GlobalConditionGenerator, FiLM

class ConvolutionalBlock(torch.nn.Module):
    def __init__(self, 
                in_channels, 
                out_channels, 
                kernel_size, 
                stride, 
                dilation,
                activation,
                cond_dim):
        super(ConvolutionalBlock, self).__init__()

        # contruct the components of the block
        self.conv = torch.nn.Conv1d(in_channels, 
                        out_channels, 
                        kernel_size, 
                        stride=stride, 
                        dilation=dilation)
        self.adpt = torch.nn.Linear(cond_dim, out_channels*2)
        self.acti = get_activation(activation)
    
    def forward(self, x, y=None):

        x = self.conv(x)

        if y is not None:
            y = self.adpt(y)
            x = FiLM(x, y)

        x = self.acti(x)

        return x
        
class ronnModel(torch.nn.Module):
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
                 init=None,
                 film_dim=2):
        super(ronnModel, self).__init__()
        
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
        self.film_dim = film_dim

        # create MLP for FiLM generation
        self.generator = GlobalConditionGenerator(self.film_dim)

        # create each block (layer) in the network
        self.blocks = torch.nn.ModuleList()
        for n in range(n_layers):

            # determeine the input/output channels
            in_channels = n_channels if n != 0 else n_inputs
            out_channels = n_channels if n != (n_layers-1) else n_outputs

            self.blocks.append(ConvolutionalBlock(in_channels,
                                                  out_channels,
                                                  kernel_size,
                                                  stride,
                                                  dilations[n],
                                                  activation,
                                                  self.generator.cond_dim))

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

    def forward(self, x, y=None):

        if y is not None:
            y = self.generator(y)   # generate the global conditioning

        for block in self.blocks:
            x_in = x  # store for residual

            if y is not None:
                x = block(x, y=y)  # process through convolutional block
            else:
                x = block(x)

            if self.residual:
                x = torch.add(x, center_crop(x_in, x.shape[-1]))

        return x
