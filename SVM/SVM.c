#include <stdio.h>
#include <string.h>
#include <sys/time.h>

double vectorTimes(int data[], double theta[], const int x)
{
	double ret = 0;
	int xCount;

	for (xCount = 0; xCount < x; xCount++)
	{
		ret += data[xCount] * theta[xCount];
	}

	return ret;
}

void printVector(double *theta, const int x)
{
	int xCount;

	for (xCount = 0; xCount < x; xCount++)
	{
		printf("%lf ", theta[xCount]);
		printf("\n");
	}
}

double testFunc(double *theta, int *data, const int x)
{
	return vectorTimes(data, theta, x);
}

int main()
{
	int data[4] = {-1, 1, -1, -1};
	const int dataItemSize = sizeof(data) / sizeof(int);
	double theta[dataItemSize];
	double learningRate = (double) 1 / dataItemSize;
	double errorSum;
	int count;
	int xCount;

	memset(theta, 0, sizeof(theta));

	for (count = 0; count < 1000; count++)
	{
		errorSum = 1 - vectorTimes(data, theta, dataItemSize);
		
		for (xCount = 0; xCount < dataItemSize; xCount++)
		{
			theta[xCount] += learningRate * errorSum * data[xCount];
		}
	}

	printVector(theta, dataItemSize);

	return 0;
}
