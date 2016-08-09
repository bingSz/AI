#include <stdio.h>

int main()
{
	int data[8][2] = {{1, -1},
			  {2, -1},
			  {3, -1},
			  {4, -1},
			  {6, 1},
			  {7, 1},
			  {8, 1},
			  {9, 1}};
	int dataSize = sizeof(data) / sizeof(data[0]);
	float theta;
	int count;
	int elementNum;
	float error;
	float learningRate = 0.01; 

	theta = 0;
	for (count = 0; count < 1000; count++)
	{
		elementNum = count % dataSize;
		
		error = data[elementNum][1] - (data[elementNum][0] + theta);
		theta += learningRate * error;

	}

	printf("%f\n", theta);

	return 0;
}
