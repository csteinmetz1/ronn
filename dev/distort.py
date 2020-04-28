import os
import sys
import time
import torch
import torchaudio
import torchsummary

from ronn.ronn import ronn
from ronn.utils import calculate_receptive_field

sample_rate = 44100

# construct the model with our desired parameters
processor = ronn(n_inputs=1,
                 n_outputs=2,
                 n_layers=30, 
                 n_channels=32,
                 kernel_size=3, 
                 stride=1, 
                 activation="ReLU", 
                 dilations=[1,2,4,8,16,32,64,128,256,512,1,2,4,8,16,32,64,128,256,512,1,2,4,8,16,32,64,128,256,512],
                 residual=False,
                 init=None, 
                 film_dim=2)

rf = calculate_receptive_field(15, 18, 1, [1,2,4,8,16,32,64,128,256,1,2,4,8,16,32,64,128,256])
print(f"{rf} samples or {(rf/sample_rate)*1000:0.2f} ms")

# take a look at the model
input_size = (1, 44100)
torchsummary.summary(processor.model, input_size)

# load some audio
x, sr = torchaudio.load("samples/clean/clean_guitar.wav")
x = x[0,:44100*4].view(1,-1) # take just the left channel since it is stereo
#y = torch.rand(1,2)
y = None

# process that audio
y = processor.process(x, y=y)

# normalize 
y /= (torch.max(torch.abs(y)))
print(y.shape)

y -= torch.mean(y,dim=-1).view(2,1)
print(torch.mean(y,dim=-1))

# save the processed audio to disk
torchaudio.save("samples/processed/p_clean_guitar.wav", y, sr)
torchaudio.save("samples/processed/c_clean_guitar.wav", x, sr)
