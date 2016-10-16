#!/bin/env python

from numpy import *
from math import *

def loadDataSet():
	dataSet = [['love', 'my', 'dog', 'right'], 
		   ['garbage', 'dimwit', 'dog', 'stupid']]
	classVect = [0, 1]

	return dataSet, classVect

def createVocabList(dataSet):
	vocabList = set([])

	for i in dataSet:
		vocabList = vocabList | set(i)

	return list(vocabList)

def setWords(vocabList, inputSet):
	returnVect = [0] * len(vocabList)

	for i in inputSet:
		if i in vocabList:
			returnVect[vocabList.index(i)] = 1
	
	return returnVect

def trainBayes(trainingMatrix, trainingFlags):
	numDocs = len(trainingMatrix)
	numWords = len(trainingMatrix[0])

	Pprior = sum(trainingFlags) / float(numDocs)
	P1Num = ones(numWords)
	P0Num = ones(numWords)
	P1All = 2.0
	P0All = 2.0

	for i in range(numDocs):
		if trainingFlags[i] == 1:
			P1Num += trainingMatrix[i]
			P1All += sum(trainingMatrix[i])
		else:
			P0Num += trainingMatrix[i]
			P0All += sum(trainingMatrix[i])

	P1Vect = P1Num / P1All
	P0Vect = P0Num / P0All

	return P1Vect, P0Vect, Pprior

def predictBayes(P1Vert, P0Vert, Pprior, vocabList, inputSet):
	dataSet = setWords(vocabList, inputSet)
	P1 = sum(P1Vert * dataSet) * Pprior
	P0 = sum(P0Vert * dataSet) * (1 - Pprior)

	if P1 > P0:
		Prob = P1 / (P1 + P0)
		return 1, Prob
	else:
		Prob = P0 / (P1 + P0)
		return 0, Prob
