import os
import sys
import time
import torch
import torchaudio
import torchsummary

from disnet.disnet import DISNET

# construct the model with our desired parameters
processor = DISNET(n_inputs=1,
                   n_outputs=1,
                   n_layers=6, 
                   n_channels=8,
                   kernel_size=256, 
                   stride=1, 
                   activation="ReLU", 
                   dilations=[1,2,4,8,16,32], 
                   init=None)

# take a look at the model
input_size = (1, 44100)
torchsummary.summary(processor.model, input_size)

# load some audio
x, sr = torchaudio.load("samples/clean/clean_guitar.wav")
x = x[0,:44100*3].view(1,-1) # take just the left channel since it is stereo

# process that audio
x = processor.process(x)

# normalize 
x /= (torch.max(torch.abs(x)) + 0.01)

# save the processed audio to disk
torchaudio.save("samples/processed/p_clean_guitar.wav", x, sr)