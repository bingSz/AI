#!/bin/env python

from numpy import *

def sgn(x):
    return 1 / (1 + exp(-x))

def BP(dataSet, labelSet, maxIter = 1000, learningRate = 0.1):
    dataMat = mat(dataSet)
    labelMat = mat(labelSet)

    m, n = shape(dataMat)
    OutNum = shape(labelMat[0])[1]
    
    hideNum = 2 * n + 1

    w0 = mat(random.uniform(-1, 1, size = (hideNum, n)))
    a = mat(random.uniform(-1, 1, size = (hideNum, 1)))
    w1 = mat(random.uniform(-1, 1, size = (OutNum, hideNum)))
    b = mat(random.uniform(-1, 1, size = (OutNum, 1)))
    
#    w0 = mat(zeros((hideNum, n)))
#    a = mat(zeros((hideNum, 1)))
#    w1 = mat(zeros((OutNum, hideNum)))
#    b = mat(zeros((OutNum, 1)))

    for Iter in xrange(maxIter):
        for i in xrange(m):
            H = sgn(w0 * dataMat[i].T + a)
            O = sgn(w1 * H + b)
            
            for j in xrange(OutNum):
                w1[j] -= multiply(learningRate * (O[j] - labelMat[i].T[j]) * O[j] * (1 - O[j]), H).T
            for j in xrange(hideNum):
                w0[j] -= multiply(learningRate * multiply((O - labelMat[i].T).T, (O.T * (1 - O))) * w1[:, j],  multiply(multiply(H[j], (1 - H[j])), dataMat[i]))

            b -= O - labelMat[i].T
            for j in xrange(hideNum):
                a[j] -= multiply(learningRate * sum(multiply((O - labelMat[i].T).T, O.T * (1 - O)) * w1[:, j]), multiply(H[j], (1 - H[j])))
            
            print O.T, labelMat[i]
    return w0, w1, a, b
            
def predict(data, w0, w1, a, b):
    dataM = mat(data)
    H = sgn(w0 * dataM.T + a)
    return w1 * H + b

