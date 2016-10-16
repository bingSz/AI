#!/bin/env python

from numpy import *

def deriv(x):
    return x * (1 - x).T
def sgn(x):
    return 1 / (1 + exp(-x))

def BP(dataSet, labelSet, learningRate = 1, maxIter = 1000):
    dataMat = mat(dataSet)
    labelMat = mat(labelSet).T

    m, n = shape(dataMat)

    if n - 1 <= 0:
        length = 1
    else:
        length = n - 1
    
    w0 = array(zeros((n, length)))
    w1 = array(zeros((length, 1)))
    a = 0

    for i in range(maxIter):
        for j in range(m):
            l0 = dataMat[j]
            l1 = sgn(dot(l0, w0))
            l2 = sgn(dot(l1, w1) + a)

            l2_error = labelMat[j] - l2

            l2_delta = l2_error * deriv(l2)

            l1_error = l2_delta.dot(w1.T)

            l1_delta = deriv(l1) * l1_error
            
            w1 += learningRate * dot(l1.T, l2_delta)
            w0 += learningRate * dot(l0.T, l1_delta)
            a += l2_error

    return w0, w1, a

def predict(w0, w1, a, dataSet):
    l0 = mat(dataSet)

    l1 = sgn(dot(l0, w0))
    l2 = sgn(dot(l1, w1) + a)
    
    return l2
