import os
import sys
import time
import torch
import torchaudio
import torchsummary

from ronn.ronn import RONN

# construct the model with our desired parameters
processor = RONN(n_inputs=1,
                 n_outputs=2,
                 n_layers=6, 
                 n_channels=4,
                 kernel_size=21, 
                 stride=1, 
                 activation="GELU", 
                 dilations=[1,2,4,8,16,32], 
                 residual=False,
                 init=None)

# take a look at the model
input_size = (1, 44100)
torchsummary.summary(processor.model, input_size)

# load some audio
x, sr = torchaudio.load("samples/clean/beat2.wav")
x = x[0,:44100*4].view(1,-1) # take just the left channel since it is stereo

# process that audio
y = processor.process(x)

# normalize 
y /= (torch.max(torch.abs(y)) + 0.1)

# save the processed audio to disk
torchaudio.save("samples/processed/p_beat2.wav", y, sr)
torchaudio.save("samples/processed/c_beat2.wav", x, sr)
