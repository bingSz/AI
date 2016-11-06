#!/bin/env python

from numpy import *

def leastSquare(X, Y, opt = 1):
    Xm = mat(X).T
    Ym = mat(Y).T

    A = []
    B = []

    if opt == 1:
        A.append([8, sum(Xm) * 2, sum(multiply(Xm, Xm) * 2)])
        B.append(sum(Ym) * 2)
        A.append([sum(Xm) * 2, sum(multiply(Xm, Xm) * 2), sum(multiply(Xm, multiply(Xm, Xm))) * 2])
        B.append(sum(multiply(Xm, Ym)) * 2)
        A.append([sum(multiply(Xm, Xm) * 2), sum(multiply(Xm, multiply(Xm, Xm))) * 2, sum(multiply(multiply(Xm, Xm) * 2, multiply(Xm, Xm)))])
        B.append(sum(multiply(multiply(Xm, Xm), multiply(Ym, 2))))
    else:
        A.append([8, sum(Xm) * 2])
        B.append(sum(Ym) * 2)
        A.append([sum(Xm) * 2, sum(multiply(Xm, Xm) * 2)])
        B.append(sum(multiply(Xm, Ym)) * 2)
        
    A = mat(A)
    B = mat(B)
    
    return B * A ** -1
