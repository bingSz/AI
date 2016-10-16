#!/bin/env python

from numpy import *

def pca(dataMat, dimCount = 100):
	meanVals = mean(dataMat, axis=0)
	meanRemoved = dataMat - meanVals
	covMat = cov(meanRemoved, rowvar=0)
	eigVals, eigVects = linalg.eig(mat(covMat))
	eigValInd = argsort(eigVals)
	eigValInd = eigValInd[:-(dimCount+1):-1]
	redEigVects = eigVects[:, eigValInd]
	lowDDataMat = meanRemoved * redEigVects
	return lowDDataMat
