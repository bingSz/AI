#include <stdio.h>
#include <string.h>

int lossValue(const int dataX, const int dataY, int data[dataX][dataY], int X, int Y, int sign[4][4])
{
	int action[4][2];
	int actionCount = 0;
	int count;
	int tempValue;
	int minValue;
	int newSign[dataX][dataY];

	if (data[X][Y] == 1)
	{
		return 0;
	}
	if (data[X][Y] < 0)
	{
		return -data[X][Y] + 1;
	}

	sign[X][Y] = 1;

	if (X + 1 < dataX && sign[X + 1][Y] != 1 && data[X + 1][Y] != 2)
	{
		action[actionCount][0] = X + 1;
		action[actionCount][1] = Y;
		actionCount += 1;
	}
	if (X - 1 >= 0 && sign[X - 1][Y] != 1 && data[X - 1][Y] != 2)
	{
		action[actionCount][0] = X - 1;
		action[actionCount][1] = Y;
		actionCount += 1;
	}
	if (Y + 1 < dataY && sign[X][Y + 1] != 1 && data[X][Y + 1] != 2)
	{
		action[actionCount][0] = X;
		action[actionCount][1] = Y + 1;
		actionCount += 1;
	}
	if (Y - 1 >= 0 && sign[X][Y - 1] != 1 && data[X][Y - 1] != 2)
	{
		action[actionCount][0] = X;
		action[actionCount][1] = Y - 1;
		actionCount += 1;
	}

	if (actionCount == 0)
	{
		return 10;
	}

	for (count = 0; count < actionCount; count++)
	{
		if (data[action[count][0]][action[count][1]] == 1)
		{
			return 1;
		}
	}

	memcpy(newSign, sign, sizeof(newSign));
	minValue = lossValue(dataX, dataY, data, action[0][0], action[0][1], newSign) + 1;
	sign[action[0][0]][action[0][1]] = 1;
	for (count = 1; count < actionCount; count++)
	{
		memcpy(newSign, sign, sizeof(newSign));
		tempValue =  lossValue(dataX, dataY, data, action[count][0], action[count][1], newSign) + 1;
		sign[action[count][0]][action[count][1]] = 1;
		if (minValue > tempValue)
		{
			minValue = tempValue;
		}
	}

	return minValue;
}

int main()
{
	int data[4][4] = {{0, 0, 0, 1}, 
			  {0, 2, 0, -1},
			  {0, 0, 0, 0}, 
			  {0, 0, 0, 0}};
	const int dataX = sizeof(data) / sizeof(data[0]);
	const int dataY = sizeof(data) / dataX / sizeof(int);

	int sign[dataX][dataY];
	int xCount;
	int yCount;
	int ret[dataX][dataY];

	for (xCount = 0; xCount < dataX; xCount++)
	{
		for (yCount = 0; yCount < dataY; yCount++)
		{
			memset(sign, 0, sizeof(sign));
			printf("%d ", lossValue(dataX, dataY, data, xCount, yCount, sign));
		}
		printf("\n");
	}
	printf("\n");

	return 0;
}
