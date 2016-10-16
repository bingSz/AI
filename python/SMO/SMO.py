#!/bin/env python

from numpy import *

class optStruct:
	def __init__(self, dataMatIn, classLabels, C, toler):
		self.X = dataMatIn
		self.labelMat = classLabels
		self.C = C
		self.tol = toler
		self.m = shape(dataMatIn)[0]
		self.alpha = mat(zeros((self.m, 1)))
		self.b = 0
		self.eCache = mat(zeros((self.m, 2)))

def calcEk(oS, k):
	fXk = float(multiply(oS.alpha, oS.labelMat).T * (oS.X * oS.X[k].T)) + oS.b
	Ek = fXk - float(oS.labelMat[k])
	return Ek

def selectJrand(i, H):
	j = i
	while j == i:
		j = int(random.uniform(0, H))

	return j

def selectJ(i, oS, Ei):
	maxK = -1
	maxDeltaE = 0
	Ej = 0
	oS.eCache[i] = [1, Ei]
	validEcacheList = nonzero(oS.eCache[:, 0].A)[0]
	if len(validEcacheList) > 1:
		for k in validEcacheList:
			if k == i:
				continue
			Ek = calcEk(oS, k)
			deltaE = abs(Ei - Ek)
			if (deltaE > maxDeltaE):
				maxK = k
				maxDeltaE = deltaE
				Ej = Ek
		return maxK, Ej
	else:
		j = selectJrand(i, oS.m)
		Ej = calcEk(oS, j)
	return j, Ej

def updateEk(oS, k):
	Ek = calcEk(oS, k)
	oS.eCache[k] = [1, Ek]

def clipAlpha(i, H, L):
	if i > H:
		i = H
	if i < L:
		i = L
	return i

def innerL(i, oS):
	Ei = calcEk(oS, i)
	if ((oS.labelMat[i] * Ei < -oS.tol) and (oS.alpha[i] < oS.C)) or ((oS.labelMat[i] * Ei > oS.tol) and (oS.alpha[i] > 0)):
		j, Ej = selectJ(i, oS, Ei)
		alphaIold = oS.alpha[i].copy()
		alphaJold = oS.alpha[j].copy()
		if (oS.labelMat[i] != oS.labelMat[j]):
			L = max(0, oS.alpha[j] - oS.alpha[i])
			H = min(oS.C, oS.C + oS.alpha[j] - oS.alpha[i])
		else:
			L = max(0, oS.alpha[j] + oS.alpha[i] - oS.C)
			H = min(oS.C, oS.alpha[j] + oS.alpha[i])
			if L == H:
				return 0
		eta = 2.0 * oS.X[i, :] * oS.X[j, :].T - oS.X[i, :] * oS.X[i, :].T - oS.X[j, :] * oS.X[j, :].T
		if eta >= 0:
			return 0

		oS.alpha[j] -= oS.labelMat[j] * (Ei - Ej) / eta
		oS.alpha[j] = clipAlpha(oS.alpha[j], H, L)
		if abs(oS.alpha[j] - alphaJold) < 0.00001:
			return 0
		oS.alpha[i] += oS.labelMat[j] * oS.labelMat[i] * (alphaJold - oS.alpha[j])
		updateEk(oS, i)
		b1 = oS.b - Ei - oS.labelMat[i] * (oS.alpha[i] - alphaIold) * oS.X[i, :] * oS.X[i, :].T - oS.labelMat[j] * (oS.alpha[j] - alphaJold) * oS.X[i, :] * oS.X[j, :].T
		b2 = oS.b - Ej - oS.labelMat[i] * (oS.alpha[i] - alphaIold) * oS.X[i, :] * oS.X[j, :].T - oS.labelMat[j] * (oS.alpha[j] - alphaJold) * oS.X[j, :] * oS.X[j, :].T
		if (0 < oS.alpha[i]) and (oS.C > oS.alpha[i]):
			oS.b = b1
		elif (0 < oS.alpha[j]) and (oS.C > oS.alpha[j]):
			oS.b = b2
		else:
			oS.b = (b1 + b2) / 2.0
		return 1
	else:
		return 0
def smoP(dataMatIn, classLabels, C, toler, maxIter):
	oS = optStruct(mat(dataMatIn), mat(classLabels).T, C, toler)
	Iter = 0
	entireSet = True
	alphaChanged = 0
	while (Iter < maxIter) and ((alphaChanged > 0) or (entireSet == True)):
		alphaChanged = 0
		if entireSet == True:
			for i in range(oS.m):
				alphaChanged += innerL(i, oS)
			Iter += 1
		else:
			nonBoundIs = nonzero((oS.alpha.A > 0) * (oS.alpha.A < C))[0]
			for i in nonBoundIs:
				alphaChanged += innerL(i, oS)
			Iter += 1
		if entireSet == True:
			entireSet = False
		elif alphaChanged == 0:
			entireSet = True
	return oS.b, oS.alpha

def calcW(alpha, dataSet, labelSet):
	dataMat = mat(dataSet)
	labelMat = mat(labelSet).T
	
	m, n = shape(dataMat)
	w = zeros((n, 1))

	for i in range(m):
		w += multiply(alpha[i] * labelMat[i], dataMat[i, :].T)
	
	return w

def SMO(dataSet, labelSet):
	b, alpha = smoP(dataSet, labelSet, 10, 0.001, 40)
	w = calcW(alpha, dataSet, labelSet)

	return b, w
