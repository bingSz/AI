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

double rounding(double src)
{
	return round(src * 100) / 100;
}

int main()
{
	double real_a, real_b, real_c;

	squareFittingRet ret;
	double data[6][2] = {{1, 12.8},		/* Suppose the function is: 9.5x^2 + 2.1x + 1.2 */
                              {2, 43.4}, 
                              {3, 93}, 
                              {4, 161.6},
                              {5, 249.2}, 
                              {6, 355.8}};

	ret = squareFitting(data, 6, 0.001, -1, 10e-4);

	real_a = rounding(ret.a);
	real_b = rounding(ret.b);
	real_c = rounding(ret.c);

	printf("%lf %lf %lf\n", real_a, real_b, real_c);	

	return 0;
}
