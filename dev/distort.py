import os
import sys
import time
import torch
import torchaudio
import numpy as np

from ronn.model import ronnModel
from ronn.utils import calculate_receptive_field

sample_rate = 44100

params = {
    "n_inputs": 2,
    "n_outputs": 2,
    "n_layers": 7,
    "n_channels": 4,
    "kernel_size": 3,
    "activation": "ReLU",
    "dilation_growth": 3,
    "init": "xavier_normal",
    "film_dim": 2,
    "seed": 4,
}

# construct the model with our desired parameters
processor = ronnModel(**params)

rf = calculate_receptive_field(
    params["kernel_size"],
    params["n_layers"],
    1,
    params["dilation_growth"] ** np.arange(params["n_layers"]),
)
print(f"{rf} samples or {(rf/sample_rate)*1000:0.2f} ms")

# load some audio
x, sr = torchaudio.load("samples/clean/clean_guitar.wav")
c, s = x.size()
x = x[:, : 44100 * 4]
x_pad = torch.nn.functional.pad(x, (rf, 0))
x_pad = x_pad.view(1, c, -1)
y = None

# process that audio
out = processor(x_pad, y=y)

# normalize
out /= out.abs().max()

# remove DC
out -= torch.mean(out, dim=-1).view(c, 1)

# save the processed audio to disk
torchaudio.save("samples/processed/p_clean_guitar.wav", out.view(c, -1), sr)
torchaudio.save("samples/processed/c_clean_guitar.wav", x.view(c, -1), sr)
