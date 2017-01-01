#!/bin/env python

from scipy.io import wavfile
from numpy import *

if __name__ == '__main__':

    sampFreq, snd = wavfile.read('a.wav')

    soundVect = []
    for i in xrange(len(snd)):
        soundVect.extend([snd[i]])

    soundTransformed = log10(clip(abs(fft.rfft(soundVect)), 1e-16, 1e100))

    soundIndex = linspace(0, sampFreq / 2, len(soundTransformed))

    IndexSorted = list(argsort(soundTransformed))
    IndexSorted.reverse()
    print soundIndex[IndexSorted[:10]]
