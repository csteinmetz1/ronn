import os
import sys
import time
import torch
import torchaudio
import torchsummary

from ronn.ronn import ronn

# construct the model with our desired parameters
processor = ronn(n_inputs=1,
                 n_outputs=2,
                 n_layers=6, 
                 n_channels=32,
                 kernel_size=32, 
                 stride=1, 
                 activation="ReLU", 
                 dilations=[1,2,4,8,16,32], 
                 residual=False,
                 init=None, 
                 film_dim=2)

# take a look at the model
input_size = (1, 44100)
torchsummary.summary(processor.model, input_size)

# load some audio
x, sr = torchaudio.load("samples/clean/clean_guitar.wav")
x = x[0,:44100*4].view(1,-1) # take just the left channel since it is stereo
y = torch.rand(1,2)
#y = None

# process that audio
y = processor.process(x, y=y)

# normalize 
y /= (torch.max(torch.abs(y)) + 0.2)

# save the processed audio to disk
torchaudio.save("samples/processed/p_clean_guitar.wav", y, sr)
torchaudio.save("samples/processed/c_clean_guitar.wav", x, sr)
