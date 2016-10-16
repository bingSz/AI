#!/bin/env python

from numpy import *
import operator
from os import listdir
import kNN

def classifyHandWriting(path, trainingPath):
	trainingFileList = listdir(trainingPath)
	listlength = len(trainingFileList)

	trainingMat = zeros((listlength, 1024))
	labels = [];

	for i in range(listlength):
		filename = trainingFileList[i]

		trainingMat[i, :] = kNN.imgVector(trainingPath + '/' + filename)

		labels.append(filename.strip().split('.')[0].split('_')[0])
	
	classification = kNN.imgVector(path)

	return kNN.classify0(classification, trainingMat, labels, 3)
