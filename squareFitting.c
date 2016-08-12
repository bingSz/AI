#include <stdio.h>

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

squareFittingRet squareFitting(double data[][2], unsigned int dataSize, double learningRate, unsigned int maxNum, double loss)
{
	squareFittingRet ret;
	double a, b, c;
	double xSquare;
	double errorSum;
	unsigned int count;
	unsigned int elementNum;

	a = 0;
	b = 0;
	c = 0;

	for (count = 0; count < maxNum; count++)
	{
		elementNum = count % dataSize;

		xSquare = data[elementNum][0] * data[elementNum][0];
		errorSum = data[elementNum][1] - (a * xSquare + b * data[elementNum][0] + c);

		if (doubleabs(errorSum) < loss)
		{
			break;
		}

		/* Update */
		a += learningRate * errorSum * xSquare;
		b += learningRate * errorSum * data[elementNum][0];
		c = data[elementNum][1] - (a * xSquare + b * data[elementNum][0]);
	}

	ret.a = a;
	ret.b = b;
	ret.c = c;

	return ret;
}

int main()
{
	squareFittingRet ret;
	double data[6][2] = {{1, 9},
                              {2, 18}, 
                              {3, 31}, 
                              {4, 48},
                              {5, 69}, 
                              {6, 94}};
	
	ret = squareFitting(data, 6, 0.01, 10000, 0.001);

	printf("%lf %lf %lf\n", ret.a, ret.b, ret.c);

	return 0;
}
