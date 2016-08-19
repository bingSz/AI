#include <stdio.h>
#include <math.h>
#include <string.h>

double vectorTimes(double *a, double *b, int size)
{
	double ret;
	int count;

	ret = 0;
	for (count = 0; count < size; count++)
	{
		ret += a[count] * b[count];
	}

	return ret;
}

int main()
{
	double data[6][2] = {{1, 2}, 
			     {1, 1}, 
			     {2, 1}, 
			     {2, 2}, 
			     {2, 3}, 
			     {1.3, 0}};
	int ret[6] = {0, 0, 1, 1, 1, 0};

	double theta[2];
	double errorSum;
	double learningRate = 0.1;
	double bias;
	int count;
	int subCount;
	int training;

	memset(theta, 0, sizeof(theta));
	bias = 0;
	for (count = 0; count < 10000; count++)
	{
		training = count % 5;

		errorSum = ret[training] - 1/(1 + exp(-vectorTimes(theta, data[training], 2) - bias));
		
		theta[0] += learningRate * errorSum * data[training][0] / 4;
		theta[1] += learningRate * errorSum * data[training][1] / 4;
		bias += errorSum;
	}

	printf("%lf\n%lf\n%f\n", theta[0], theta[1], bias);

	return 0;
}
