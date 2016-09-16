#include <stdio.h>
#include <math.h>

/*
 * How to use function squareFitting?
 * the first argument is data, the data is 2-dim transpose
 * the second argument is dataSize, the dataSize is the dimension of the data
 * the third argument is learningRate, the learningRate is the rate of learning, learningRate should be between 0 and 1
 * the fourth argument is maximum number of times to do
 * the fifth argument is the loop exit, when the errorSum smaller than loss
 * */

typedef struct
{
	double a;
	double b;
	double c;
}squareFittingRet;

double doubleabs(double num)
{
	if (num > 0)
	{
		return num;
	}
	else
	{
		return -num;
	}
}

squareFittingRet squareFitting(double data[][2], unsigned int dataSize, double learningRate, int maxNum, double loss)
{
	squareFittingRet ret;
	double a, b, c;
	double xSquare;
	double errorSum;
	double allErrorSum;
	unsigned int count;
	unsigned int numCount;
	unsigned int elementNum;
	int addNum = 1;
	int subCount;

	a = 0;
	b = 0;
	c = 0;

	if (maxNum == -1)
	{
		addNum = 0;
		maxNum = 1;
	}
	for (count = 0, numCount = 0; count < maxNum; count += addNum, numCount += 1)
	{
		elementNum = numCount % dataSize;

		xSquare = data[elementNum][0] * data[elementNum][0];
		errorSum = data[elementNum][1] - (a * xSquare + b * data[elementNum][0] + c);
	
		allErrorSum = 0;
		for (subCount = 0; subCount < dataSize; subCount++)
		{
			allErrorSum += fabs(data[subCount][1] - (a * data[subCount][0] * data[subCount][0] + b * data[subCount][0] + c));
		}

		if (fabs(allErrorSum) <= loss)
		{
			break;
		}

		/* Update */
		a += learningRate * errorSum * xSquare;
		b += learningRate * errorSum * data[elementNum][0];
		c += errorSum;
	}

	ret.a = a;
	ret.b = b;
	ret.c = c;

	return ret;
}

int main()
{
	squareFittingRet ret;
	double data[6][2] = {{1, 12},		/* Suppose the function is: 9x^2 + 2x + 1 */
                              {2, 41}, 
                              {3, 88}, 
                              {4, 153},
                              {5, 236}, 
                              {6, 337}};

	ret = squareFitting(data, 6, 0.001, -1, 10e-10);

	printf("%lf %lf %lf\n", ret.a, ret.b, ret.c);	

	return 0;
}
