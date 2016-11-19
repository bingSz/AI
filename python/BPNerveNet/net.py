#!/bin/env python

from numpy import *

def sgn(x):
    return 1 / (1 + exp(-x))

def BP(dataSet, labelSet, maxIter = 100, learningRate = 0.1):
    dataMat = mat(dataSet)
    labelMat = mat(labelSet).T

    m, n = shape(dataMat)

    hideNum = 2 * n + 1
    
    w0 = mat(zeros((hideNum, n)))
    a = mat(zeros((hideNum, 1)))
    w1 = mat(zeros((hideNum, 1)))
    b = mat(zeros((1, 1)))

    for Iter in xrange(maxIter):
        for i in xrange(m):
            H = sgn(w0 * dataMat[i].T + a)
            O = sgn(w1.T * H + b)
            
            w1 -= multiply(learningRate * (O - labelMat[i]) * O * (1 - O), H)
            for j in range(hideNum):
                w0[j] -= multiply(learningRate * (O - labelMat[i]) * O * (1 - O) * w1[j],  multiply(multiply(H[j], (1 - H[j])), dataMat[i]))
            a -= multiply(learningRate * (O - labelMat[i]) * (O * (1 - O)) * w1[1], multiply(H, (1 - H)))
            b -= O - labelMat[i]
    return w0, w1, a, b
            
def predict(data, w0, w1, a, b):
    dataM = mat(data)
    H = sgn(w0 * dataM.T + a)
    return sgn(w1.T * H + b)

