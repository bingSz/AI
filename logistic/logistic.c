#include <stdio.h>
#include <string.h>
#include <math.h>

double vectorTimes(double data[][2], double theta[][2], const int x)
{
	double ret = 0;
	int xCount;
	int yCount;

	for (xCount = 0; xCount < x; xCount++)
	{
		for (yCount = 0; yCount < 2; yCount++)
		{
			ret += data[xCount][yCount] * theta[xCount][yCount];
		}
	}

	return ret;
}

void printVector(double (*theta)[2], const int x, const int y)
{
	int xCount;
	int yCount;

	for (xCount = 0; xCount < x; xCount++)
	{
		for (yCount = 0; yCount < y; yCount++)
		{
			printf("%lf ", theta[xCount][yCount]);
		}
		printf("\n");
	}
}

double testFunc(double (*theta)[2], double (*data)[2], const int x)
{
	return vectorTimes(theta, data, x);
}

double computeProbability(double value)
{
	return 1 / (1 + exp(-value));
}

int main()
{
	double data[4][2] = {{2, 1}, 
			     {1, 4}, 
			     {5, 1}, 
			     {3, 3}};
	const int dataItemSize = sizeof(data) / sizeof(data[0]);
	double theta[dataItemSize][2];
	double learningRate = 0.01;
	double errorSum;
	int count;
	int xCount;
	int yCount;
	int elementNum;

	memset(theta, 0, sizeof(theta));

	for (count = 0; count < 100; count++)
	{
		elementNum = count % dataItemSize;

		errorSum = 1 - vectorTimes(data, theta, dataItemSize); 
	
		for (xCount = 0; xCount < dataItemSize; xCount++)
		{
			for (yCount = 0; yCount < 2; yCount++)
			{
				theta[xCount][yCount] += learningRate * errorSum * data[xCount][yCount];
			}
		}
	}

	printf("-----The result-----\n");
	printVector(theta, dataItemSize, 2);
	printf("-----End------------\n");

	printf("-----The test data--\n");
	double testData[4][2] = {{1, 2}, 
				 {3, 1},
				 {0, 5}, 
				 {1, 0}};
	printVector(testData, 4, 2);
	printf("-----End------------\n");
	
	printf("-----The test result\n");
	printf("The probability is: %lf\n", computeProbability(testFunc(theta, testData, dataItemSize)));
	printf("-----End------------\n");

	return 0;
}
