#!/bin/env python

from numpy import *

def selectJrand(i, m):
	j = i
	while (j == i):
		j = int(random.uniform(0, m))

	return j

def clipAlpha(aj, H, L):
	if aj > H:
		aj = H
	if L > aj:
		aj = L

	return aj

def smoSimple(dataSetIn, labelsIn, C, toler, maxIter):
	dataMat = mat(dataSetIn)
	labelMat = mat(labelsIn).transpose()

	b = 0
	m, n = shape(dataMat)
	alpha = mat(zeros((m, 1)))

	Iter = 0

	while Iter < maxIter:
		alphaChanged = 0
		for i in range(m):
			fXi = float(multiply(alpha, labelMat).T * (dataMat * dataMat[i, :].T)) + b
			Ei = fXi - float(labelMat[i])
			if ((labelMat[i] * Ei < -toler) and (alpha[i] < C)) or ((labelMat[i] * Ei > toler) and (alpha[i] > 0)):
				j = selectJrand(i, m)
				fXj = float(multiply(alpha, labelMat).T * (dataMat * dataMat[j, :].T)) + b
				Ej = fXj - float(labelMat[j])

				alphaIold = alpha[i].copy()
				alphaJold = alpha[j].copy()

				if labelMat[i] != labelMat[j]:
					L = max(0, alpha[j] - alpha[i])
					H = min(C, C + alpha[j] - alpha[i])
				else:
					L = max(0, alpha[j] + alpha[i] - C)
					H = min(C, alpha[j] + alpha[i])

				if L == H:
					continue
				eta = 2.0 * dataMat[i, :] * dataMat[j, :].T - dataMat[i, :] * dataMat[i, :].T - dataMat[j, :] * dataMat[j, :].T

				if eta >= 0:
					continue

				alpha[j] -= labelMat[j] * (Ei - Ej) / eta
				alpha[j] = clipAlpha(alpha[j], H, L)
				if (abs(alpha[j] - alphaJold) < 0.00001):
					continue
				alpha[i] += labelMat[j] * labelMat[i] * (alphaJold - alpha[j])

				b1 = b - Ei - labelMat[i] * (alpha[i] - alphaIold) * dataMat[i, :] * dataMat[i, :].T - labelMat[j] * (alpha[j] - alphaJold) * dataMat[i, :] * dataMat[j, :].T
				b2 = b - Ej - labelMat[i] * (alpha[i] - alphaIold) * dataMat[i, :] * dataMat[j, :].T - labelMat[j] * (alpha[j] - alphaJold) * dataMat[j, :] * dataMat[j, :].T
				if (0 < alpha[i]) and (C > alpha[i]):
					b = b1
				elif (0 < alpha[j]) and (C > alpha[j]):
					b = b2
				else:
					b = (b1 + b2) / 2.0
				alphaChanged += 1	

		if alphaChanged == 0:
			Iter += 1
		else:
			Iter = 0

	return b, alpha

def calcWv(alpha, dataSet, labelSet):
	dataMat = mat(dataSet)
	labelMat = mat(labelSet).T

	m, n = shape(dataMat)

	Wv = zeros((n, 1))

	for i in range(m):
		Wv += multiply(alpha[i] * labelMat[i], dataMat[i].T)
	
	return Wv

def SMO(dataSet, labelSet):
	b, alpha = smoSimple(dataSet, labelSet, 10, 0.001, 10)
	Wv = calcWv(alpha, dataSet, labelSet)

	return b, mat(Wv)
