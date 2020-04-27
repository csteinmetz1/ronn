import torch

class GlobalConditionGenerator(torch.nn.Module):
    """ Simple MLP that generates a global conditioning vector. 

    Args:
        input_dim (int): dimensions of the input vector.
        cond_dim (int): dimensions of the generated global conditioning vector. 
        n_layers (int): linear layers in the MLP generator. 

    """
    def __init__(self, input_dim, n_layers=3, cond_dim=128):
        super(GlobalConditionGenerator, self).__init__()

        self.input_dim = input_dim
        self.n_layers = n_layers
        self.cond_dim = cond_dim

        self.layers = torch.nn.ModuleList()

        for n in range(n_layers):

            if n == 0: 
                in_features = input_dim
                out_features = input_dim**2
            elif n+1 == n_layers:
                in_features = out_features
                out_features = cond_dim
            else:
                in_features = out_features
                out_features = in_features**2

            block = torch.nn.Sequential(
                torch.nn.Linear(in_features, out_features),
                torch.nn.ReLU()
            )

            self.layers.append(block)

            print(in_features, out_features)

    def forward(self, x):

        for layer in self.layers:
            x = layer(x)

        return x

def FiLM(x, y):

    bs,p = y.size()

    g = y[...,:p//2].view(bs,-1,1)
    b = y[...,p//2:].view(bs,-1,1)

    # apply film transform
    x = torch.mul(g, x) + b

    return x