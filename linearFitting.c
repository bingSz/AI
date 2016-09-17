#include <stdio.h>
#include <math.h>

int main()
{
	float matrix[4] = {1, 2, 3, 4};
	float ret[4] = {3, 5, 7, 9};
	float theta = 1.4;
	float bias = 3.5;
	float h;
	float errorSum;
	float learningRate = 0.01;
	int i, j;

	for (i = 0; i < 100000; i++)
	{
		j = i % 4;

		errorSum = ret[j] - theta * matrix[j] - bias;

		theta += learningRate * matrix[j] * errorSum;
		bias += errorSum;
	}

	printf("%f %f\n", theta, bias);

	return 0;
}
