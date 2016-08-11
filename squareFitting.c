#include <stdio.h>

int main()
{
	double a, b, c;
	/* Assume that the function is: y = 2x^2 + 3x + 4 */
	double data[6][2] = {{1, 9},
			     {2, 18}, 
			     {3, 31}, 
			     {4, 48},
			     {5, 69}, 
			     {6, 94}};
	double xSquare;
	double errorSum;
	double learningRate;
	int dataSize = sizeof(data) / sizeof(data[0]);
	int count;
	int elementNum;

	a = 0;
	b = 0;
	c = 0;
	learningRate = 0.01;

	for (count = 0; count < 100000; count++)
	{
		elementNum = count % dataSize;

		xSquare = data[elementNum][0] * data[elementNum][0];
		errorSum = data[elementNum][1] - (a * xSquare + b * data[elementNum][0] + c);

		/* Update */
		a += learningRate * errorSum * xSquare;
		b += learningRate * errorSum * data[elementNum][0];
		c = data[elementNum][1] - (a * xSquare + b * data[elementNum][0]);
	}

	printf("%lf %lf %lf\n", a, b, c);

	return 0;
}
