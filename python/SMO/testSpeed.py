#!/bin/env python

from timeit import *
import pickle

import SMO
import simpleSMO

if __name__ == '__main__':
	dataSet = pickle.load(open('dataSet'))
	labelSet = pickle.load(open('labelSet'))

	for i in range(len(dataSet)):
		for j in range(len(dataSet[0])):
			dataSet[i][j] = float(dataSet[i][j])

	for i in range(len(labelSet)):
		labelSet[i] = float(labelSet[i])

	print 'The first time of simpleSMO: ' + str(Timer('simpleSMO.SMO(dataSet, labelSet)', 'from __main__ import simpleSMO, dataSet, labelSet').timeit(1))
	print 'The second time of simpleSMO: ' + str(Timer('simpleSMO.SMO(dataSet, labelSet)', 'from __main__ import simpleSMO, dataSet, labelSet').timeit(1))
	print 'The third time of simpleSMO: ' + str(Timer('simpleSMO.SMO(dataSet, labelSet)', 'from __main__ import simpleSMO, dataSet, labelSet').timeit(1))
	
	print 'The first time of SMO: ' + str(Timer('SMO.SMO(dataSet, labelSet)', 'from __main__ import SMO, dataSet, labelSet').timeit(1))
	print 'The second time of SMO: ' + str(Timer('SMO.SMO(dataSet, labelSet)', 'from __main__ import SMO, dataSet, labelSet').timeit(1))
	print 'The third time of SMO: ' + str(Timer('SMO.SMO(dataSet, labelSet)', 'from __main__ import SMO, dataSet, labelSet').timeit(1))
