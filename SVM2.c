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

int main()
{
	int data[8][2] = {{1, 1}, 
			  {1, 2}, 
			  {2, 2}, 
			  {2, 1}, 
			  {0, 1}, 
			  {0, 2}, 
			  {3, 1}, 
			  {3, 2}};
	int y[8] = {-1, -1, 1, 1, -1, -1, 1, 1};
	const int dataItemSize = sizeof(data) / sizeof(data[0]);
	const int dataSubItemSize = sizeof(data) / dataItemSize / sizeof(int);
	double theta[dataSubItemSize];
	double bias = 0;
	double learningRate = (double) 1 / dataItemSize;
	double errorSum;
	int count;
	int xCount;
	int element;

	memset(theta, 0, sizeof(theta));

	for (count = 0; count < 4000; count++)
	{
		element = count % dataItemSize;
		
		errorSum = y[element] - (vectorTimes(data[element], theta, dataSubItemSize));
		
		for (xCount = 0; xCount < dataSubItemSize; xCount++)
		{
			theta[xCount] += learningRate * errorSum * data[element][xCount];
		}

		bias = y[element] - (vectorTimes(data[element], theta, dataSubItemSize));
	}

	printVector(theta, dataSubItemSize);
	printf("Bias: %lf\n", bias);

	return 0;
}
