#!/bin/env python

from numpy import *

def sgn(x):
    return 1 / (1 + exp(-x))

def predict(data, w, a):
    if sgn(w.T * mat(data).T + a) > 0.5:
        return 1
    else:
        return -1

def learning(dataSet, labelSet, w, a):
    dataMat = mat(dataSet)
    labelMat = mat(labelSet).T

    m, n = shape(dataMat)

    for i in range(m):
        sign = predict(dataMat[i], w, a)
        if sign != labelMat[i]:
            w += multiply(labelMat[i], dataMat[i].T)
            a += labelMat[i] - sign
