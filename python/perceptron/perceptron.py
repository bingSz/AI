#!/bin/env python

from numpy import *

def sgn(x):
    return 1 / (1 + exp(-x))

def perceptron(dataSet, labelSet, learningRate = 0.1, maxIter = 1000):
    dataMat = mat(dataSet)
    labelMat = mat(labelSet).T

    n, m = shape(dataMat)
    
    w = mat(zeros((m, 1)))
    a = 0

    for Iter in range(maxIter):
        for i in range(n):
            o = sgn(w.T * dataMat[i].T + a)
            e = labelMat[i] - o

            w += multiply(learningRate * e, dataMat[i].T)
            
            a += e

    return w, a

def predict(dataSet, w, a):
    if sgn(w.T * mat(dataSet).T + a) > 0.5:
        return 1
    else:
        return 0
