import os
import sys	
import glob 					# grab files
import imageio					# create animated gifs
import cairosvg					# convert svg to png
import numpy as np				# yep
import seaborn as sns			# make plots look pretty
import soundfile as sf 			# write audio files
import matplotlib.pyplot as plt	# make plots
from scipy import signal		# correlate function
from scipy import fftpack		# fft functions

def sound_ft(filepath, name="sound", samples=262144):
	print(f"Analyze {os.path.basename(filepath)} using 'Fourier' Transform...")
	if not os.path.isdir(f"../img/{name}"):
		os.makedirs(f"../img/{name}/svg")
		os.makedirs(f"../img/{name}/png")

	# load piano audio
	piano, rate = sf.read(filepath)
	piano = piano[:len(samples)]

	fig = plt.figure(figsize=(10, 8))
	ax1 = fig.add_subplot(2,1,1)
	ax2 = fig.add_subplot(2,1,2)
	fourier = []
	freqs = np.arange(20, 1000, 20)
	for idx, freq in enumerate(freqs):
		xcorr_lags = 2000
		kernal = np.sin(2 * np.pi * freq * samples)
		xcorr = signal.correlate(piano[:xcorr_lags], kernal[:xcorr_lags], mode='full')
		xcorr /= xcorr.shape[0] 
		ax1.plot(xcorr[xcorr_lags:], 'teal')
		ax1.set_ylim(-0.1, 0.1)
		ax1.set_xlim(0, xcorr_lags)
		ax1.fill_between(np.arange(xcorr_lags-1), 0, 
			xcorr[xcorr_lags:], facecolor='lightcyan')
		fourier.append(np.sum(np.abs(xcorr[xcorr_lags:])))
		ax2.plot(freqs[:len(fourier)], fourier, color='firebrick', zorder=1)
		ax2.scatter(freq, fourier[idx], color='maroon', zorder=10)
		ax2.text(125, 35, "C4 = 261 Hz", verticalalignment='center')
		ax2.axvline(x=261, color='lightslategrey')
		ax2.text(freq+3, fourier[idx]+2, "f = {} Hz".format(freq), verticalalignment='center')		
		ax2.set_ylim(0, 40)
		ax2.set_xlim(20, 1010)
		sns.despine()
		plt.savefig(f"../img/{name}/svg/{idx}_piano_{freq}_corr.svg")
		ax1.clear() 
		ax2.clear()
	
	# turn into png ...
	svg_frames = glob.glob(f"../img/{name}/svg/*.svg")
	png_dir = f"../img/{name}/png"
	for frame in svg_frames:
		frame_name = os.path.basename(frame).replace(".svg", ".png")
		cairosvg.svg2png(url=frame, write_to=os.path.join(png_dir, frame_name))

	# ... then into gif
	png_frames = sorted(glob.glob(f"../img/{name}/png/*.png"), key=find_idx)
	output_file = f"../img/{name}.gif"
	duration = 0.1 #* len(png_frames)
	create_gif(png_frames, output_file, duration)

def create_gif(filenames, output_file, duration):
    images = []
    for filename in filenames:
        images.append(imageio.imread(filename))
    imageio.mimsave(output_file, images, duration=duration)

def find_idx(filename):
	return int(os.path.basename(filename).split('_')[0])

if __name__ == "__main__":
    sound_ft("../audio/eguitar.mp3", name="eguitar")