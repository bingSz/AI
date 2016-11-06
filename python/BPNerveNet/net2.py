#!/bin/env python

from numpy import *

def sgn(x):
    return 1 / (1 + exp(-x))

def BP(dataSet, labelSet, learningRate = 1, maxIter = 1000):
    dataMat = mat(dataSet)
    labelMat = mat(labelSet).T

    m = shape(dataMat)[1]

    if m < 2:
        numOfH = 2
    else:
        numOfH = m - 1
    numOfO = shape(labelMat[0])[1]
        
    H = mat(zeros((numOfH, 1)))
    w0 = mat(zeros((numOfH, m)))
    w1 = mat(zeros((numOfO, numOfH)))
    O = mat(zeros((numOfO, 1)))
    E = mat(zeros((numOfO, 1)))
    a = mat(zeros((numOfH, 1)))
    b = mat(zeros((numOfO, 1)))
    
    for Iter in range(maxIter):
        for i in range(m):
            for j in range(numOfH):
                H[j] = sgn(w0[j] * dataMat[i].T + sum(a))
            for j in range(numOfO):
                O[j] = H.T * w1[j].T + sum(b)
            E = labelMat[i].T - O
            
            for k in range(numOfO):
                for j in range(numOfH):
                    w1[k].T[j] += learningRate * H[j] * E[k]
            b += E

            for j in range(numOfH):
                for k in range(m):
                    sumOfEW = w1[:, j] * E
                    w0[j].T[k] += learningRate * H[j] * (1 - H[j]) * dataMat[i].T[k] * sumOfEW
            for j in range(numOfH):
                sumOfEW = w1[:, j] * E
                a[j] += learningRate * H[j] * (1 - H[j]) * sumOfEW
    return w0, w1, a, b

def predict(w0, w1, a, b, data):
    dataMat = mat(data)
    m = shape(w0)[0]
    n = shape(w1)[0]

    H = mat(zeros((m, 1)))
    O = mat(zeros((n, 1)))
    
    for j in range(m):
        H[j] = sgn(w0[j] * dataMat.T + sum(a))
    for j in range(n):
        O[j] = H.T * w1[j].T + sum(b)

    return O
    
