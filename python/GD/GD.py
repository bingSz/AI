#!/bin/env python

from numpy import *

def calcE(X, Y, j, w, a):
    m = shape(X)[0]
    e = 0

    for i in range(m):
        e += ((Y[i] - w.T * X[i].T - a) ** 2) * X[i][j]

    e *= 1 / float(m)
        
    return e

def GD(X, Y, learningRate = 0.1, maxIter = 100, eta = 0):
    Xm = mat(X)
    Ym = mat(Y).T

    n, m = shape(Xm)
    
    w = mat(zeros((m, 1)))
    a = 0

    lastChanged = mat(zeros((m, 1)))
    
    for Iter in xrange(maxIter):
        for i in xrange(m):
            e = calcE(Xm, Ym, i, w, a)
            w[i] += learningRate * e + lastChanged[i] * eta
            lastChanged[i] = learningRate * e

        t = 0
        for j in xrange(n):
            t += Ym[j] - w.T * Xm[j].T - a

        a += t / float(n)
            
    return a, w
