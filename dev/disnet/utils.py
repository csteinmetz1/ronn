import torch

def get_activation(activation_type):
    """ Give a string defining the activation and return the function itself.

    Args:
        activation_type (str): valid string defining an activation function.

    Returns:
        activation_func (obj): PyTorch activation function. 

    Valid activation functions include:
    ['ReLU', 'LeakyReLU', 'Tanh', 'Sigmoid']
    """

    if activation_type ==  "ReLU":
        activation_func = torch.nn.ReLU()
    else:
        raise ValueError(f"Invalid activation '{activation_type}'.")

    return activation_func