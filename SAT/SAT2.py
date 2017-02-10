#!/bin/env python

from numpy import *

def sgn(x):
    return 1 / (1 + exp(-5 * x))

def calcG(X, Ind, w):
    Xmat = mat(X)
    IndMat = mat(Ind)
    Wmat = mat(w)

    gain = 0;
    
    for i in xrange(len(Xmat)):
        t = array(Xmat[i] - Wmat[IndMat[i]].T)
        t = sgn(sum(1 - t * t))
        gain += t

    for i in xrange(len(Wmat)):
        gain -= 10 * (Wmat[i] ** 2 - Wmat[i]) ** 2

    return gain

def calcRight(Xmat, Ind, Wmat):
    gain = 0
    for i in xrange(len(Xmat)):
        if Xmat[i, 0] == Wmat[Ind[i, 0]] or Xmat[i, 1] == Wmat[Ind[i, 1]] or Xmat[i, 2] == Wmat[Ind[i, 2]]:
            gain += 1
    return gain
                                                               

def calcJ(Wmat):
    t = array(0.5 - Wmat) ** 2
    return sum(sgn(-100 * t))

def calcD(Wmat, Xmat, Ind, i):
    t1 = 0
    for j in xrange(len(Xmat)):
        if i not in Ind[j]:
            continue
        t = array(Xmat[j] - Wmat[mat(Ind)[j]].T)
        t = sgn(sum(1 - t * t))
        t1 += t * (1 - t) * (2 * Xmat[j, Ind[j].index(i)] - 2 * Wmat[i])
        
    return t1

def correctW(Wmat):
    t = Wmat.copy()
    t[t > 0.5] = 1;
    t[t <= 0.5] = 0;
    return t

def SAT2(X, Ind):
    Xmat = mat(X)
    IndMat = mat(Ind)
    Wmat = mat(ones((IndMat.max() + 1, 1)) * 0.5)
    
    eta = 10
    
    for counter in xrange(100):
        changedTotal = 0
        for i in xrange(len(Wmat)):
            # First step: calculate all of the data
            t = calcD(Wmat, Xmat, Ind, i)
            Wmat[i] += 0.1 * t
            changedTotal += t ** 2
        print counter
       # print Wmat
        if changedTotal <= 0.00001:
            break

    for i in xrange(len(Wmat)):
        if Wmat[i] - 0.5 > 0.0001:
            continue
        t = Wmat.copy()
        t[i] += 0.2
        t2 = Wmat.copy()
        t2[i] -= 0.2
        if abs(calcD(t, Xmat, Ind, i)) > abs(calcD(t2, Xmat, Ind, i)):
            Wmat = t2.copy()
        else:
            Wmat = t.copy()

    Wmat = correctW(Wmat)
            
    print Wmat
    print calcRight(Xmat, IndMat, Wmat)
