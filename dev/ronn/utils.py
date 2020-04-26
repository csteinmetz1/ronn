import torch

def center_crop(x, size):
    """ Given a tensor x, crop the center most portion along the last dim
        such as to match the provdied size. 

    Args:
        x (tensor): tensor to crop.
        size (int): desired output size of the last dimension.

    Returns 
        x (tensor): cropped tensor.
    """

    s = (x.shape[-1] - size)//2
    e = s + size
    
    return x[...,s:e]


def get_activation(activation_type):
    """ Give a string defining the activation and return the function itself.

    Args:
        activation_type (str): valid string defining an activation function.

    Returns:
        activation_func (obj): PyTorch activation function. 

    Experimental results
    ----------------------
    - ReLU: solid distortion
    - LeakyReLU: somewhat veiled sound
    - Tanh: insane levels of distortion with lots of aliasing (HF)
    - Sigmoid: too gritty to be useful
    - ELU: fading in and out
    - RReLU: really interesting HF noise with a background sound
    - SELU: rolled off soft distortion sound
    - GELU: roomy, not too interesting
    - Softplus: heavily distorted signal but with a very rolled off sound. (nice)
    - Softshrink: super distant sounding and somewhat roomy
    """

    if activation_type ==  "ReLU": 
        activation_func = torch.nn.ReLU()
    elif activation_type == "LeakyReLU":
        activation_func = torch.nn.LeakyReLU()
    elif activation_type == "Tanh":
        activation_func = torch.nn.Tanh()
    elif activation_type == "Sigmoid":
        activation_func = torch.nn.Sigmoid()
    elif activation_type == "ELU":
        activation_func = torch.nn.ELU()
    elif activation_type == "RReLU":
        activation_func = torch.nn.RReLU()
    elif activation_type == "SELU":
        activation_func = torch.nn.SELU()
    elif activation_type == "GELU":
        activation_func = torch.nn.GELU()
    elif activation_type == "Softplus":
        activation_func = torch.nn.Softplus()
    elif activation_type == "Softshrink":
        activation_func = torch.nn.Softshrink()
    else:
        raise ValueError(f"Invalid activation '{activation_type}'.")

    return activation_func