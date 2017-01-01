#!/bin/env python

import sys
from scipy.io import wavfile
from numpy import *

if __name__ == '__main__':

    if len(sys.argv[1]) == 0:
        sampFreq, snd = wavfile.read('1.wav')
    else:
        sampFreq, snd = wavfile.read(sys.argv[1])

    soundVect = []
    for i in xrange(len(snd)):
        soundVect.extend([snd[i]])

    soundTransformed = log10(clip(abs(fft.rfft(soundVect)), 1e-16, 1e100))

    soundIndex = linspace(0, sampFreq / 2, len(soundTransformed))

    IndexSorted = list(argsort(soundTransformed))
    IndexSorted.reverse()
    print soundIndex[IndexSorted[:100]]
