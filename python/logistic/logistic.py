#!/bin/env python

from numpy import *

def sgn(x):
    return 1 / (1 + exp(-x))

def logistic(X, Y, learningRate = 0.1, maxIter=100):
    Xmat = mat(X)
    Ymat = mat(Y).T

    m, n = shape(Xmat)

    w = mat(random.uniform(0, 1, (n, 1)))
    b = random.uniform(0, 1)

    for Iter in xrange(maxIter):
        for i in xrange(m):
            outputY = sgn(w.T * Xmat[i].T + b)
            error = Ymat[i] - outputY

            w += multiply(error * outputY * (1 - outputY), Xmat[i].T)
            b += error

            print error

    return w, b

def predict(X, w, b):
    Xmat = mat(X)
    t = sgn(w.T * Xmat.T + b)
    return t.tolist()[0][0], int(round(t))
