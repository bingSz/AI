#!/bin/env python

from numpy import *

def loadSimpData():
	dataMat = mat([[1, 2.1], [2, 1.1], [1.3, 1], [1, 1], [2, 1]])
	labelSet = [1, 1, -1, -1, 1]

	return dataMat, labelSet
