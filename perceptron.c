#include <stdio.h>

int main()
{
	int data[8][3] = {{-1, 1, -1},
			  {-2, 2, -1},
			  {-3, 3, -1},
			  {-4, 4, -1},
			  {1, 2, 1},
			  {2, 3, 1},
			  {3, 4, 1},
			  {4, 5, 1}};
	int dataSize = sizeof(data) / sizeof(data[0]);
	float theta[2];
	int count;
	int elementNum;
	float error;
	float learningRate = 0.01; 

	theta[0] = 0;
	theta[1] = 0;
	for (count = 0; count < 1000; count++)
	{
		elementNum = count % dataSize;
		
		error = data[elementNum][2] - (data[elementNum][0] * theta[0]) - (data[elementNum][1] * theta[1]);
		theta[0] += learningRate * error * data[elementNum][0];
		theta[1] += learningRate * error * data[elementNum][1];
	
	}

	printf("%f\n%f\n", theta[0], theta[1]);

	return 0;
}
