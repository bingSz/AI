#!/bin/env python

from __future__ import division
from numpy import *
import operator

def classify0(inX, dataSet, labels, k):
	dataSetSize = dataSet.shape[0]
	diffMat = tile(inX, (dataSetSize, 1)) - dataSet
	sqMat = diffMat ** 2
	sqSum = sqMat.sum(axis=1)
	distances = sqSum ** 0.5
	sortedDistances = distances.argsort()

	classCount = {}
	for i in range(k):
		voteLabel = labels[sortedDistances[i]]
		classCount[voteLabel] = classCount.get(voteLabel, 0) + 1

	sortedClassCount = sorted(classCount.iteritems(), key = operator.itemgetter(1), reverse = True)

	return sortedClassCount[0][0]

def autoNorm(dataSet):
	maxValue = dataSet.max(0)
	minValue = dataSet.min(0)
	deltaValue = maxValue - minValue
	
	dataSetNorm = zeros(shape(dataSet))
	length = dataSet.shape[0]
	dataSetNorm = dataSet - tile(minValue, (length, 1))
	dataSetNorm = dataSetNorm / tile(deltaValue, (length, 1))

	return dataSetNorm

def imgVector(path):
	returnVect = zeros((1, 1024))
	fr = open(path)
	for i in range(32):
		linestr = fr.readline()
		for j in range(32):
			returnVect[0, i * 32 + j] = linestr[j]

	return returnVect

