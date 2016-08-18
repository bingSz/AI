#include <stdio.h>
#include <math.h>

int main()
{
	float matrix[10] = {0.838797, 0.546369, 0.191803, 0.117343, 0.425687, 0.020788, 0.709541, 0.722955, 0.147015, 0.143902};
	float ret[10] = {0.913639, 0.014402, 0.400167, 0.403688, 0.681501, 0.201632, 0.491069, 0.224068, 0.328116, 0.339887};
	float theta = 0;
	float h;
	float errorSum;
	float learningRate = 0.0001;
	int i, j;

	for (i = 0; i < 1000000; i++)
	{
		j = i % 10;

		h = (matrix[j] * theta) * (matrix[j] * theta);

		errorSum = ret[j] - h;
		theta += learningRate * matrix[j] * errorSum;
	}

	printf("%f\n", theta);

	return 0;
}
