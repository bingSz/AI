#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define PUNISHMENT 10

int main()
{
	float all[3][4] = {{0, 0, 0, 'E'}, 
			   {0, 0, 0, 'M'}, 
			   {0, 0, 0, 0}};
	const int allY = sizeof(all) / sizeof(all[0]);
	const int allX = sizeof(all) / allY / sizeof(float);
	float ret[allY][allX];
	int allLoop1, allLoop2;
	int allAction[4][2];
	int nowAction;
	int allPastAction[allY][allX];
	int arrayNum;
	int targetX, targetY;
	int value;
	float firstNum;

	struct timeval seed;

	memcpy(ret, all, sizeof(ret));

	for (allLoop1 = 0; allLoop1 < allY; allLoop1++)
	{
		for (allLoop2 = 0; allLoop2 < allX; allLoop2++)
		{
			if (ret[allLoop1][allLoop2] == 'M')
			{
				continue;
			}
			else if (ret[allLoop1][allLoop2] == 'E')
			{
				continue;
			}
			else if (ret[allLoop1][allLoop2] == 'B')
			{
				continue;
			}

			targetX = allLoop1;
			targetY = allLoop2;
			value = 0;
			memset(allPastAction, 0, sizeof(allPastAction));
			while (ret[targetX][targetY] != 'E')
			{
				memset(allAction, 0, sizeof(allAction));
				allPastAction[targetX][targetY] = 1;

				arrayNum = 0;
				if (ret[targetX][targetY] == 'B')
				{
					value -= 1;
				}

				if (targetX + 1 <= allY &&
					allPastAction[targetX + 1][targetY] != 1 &&
					ret[targetX + 1][targetY] != 2)
				{
					allAction[arrayNum][0] = targetX + 1;
					allAction[arrayNum][1] = targetY;
					arrayNum += 1;
				}
				if (targetX - 1 >= 0 &&
					allPastAction[targetX - 1][targetY] != 1 &&
					ret[targetX - 1][targetY] != 2)
				{
					allAction[arrayNum][0] = targetX - 1;
					allAction[arrayNum][1] = targetY;
					arrayNum += 1;
				}
				if (targetY + 1 <= allX &&
					allPastAction[targetX][targetY + 1] != 1 &&
					ret[targetX][targetY + 1] != 2)
				{
					allAction[arrayNum][0] = targetX;
					allAction[arrayNum][1] = targetY + 1;
					arrayNum += 1;
				}
				if (targetY - 1 >= 0 &&
					allPastAction[targetX][targetY - 1] != 1 &&
					ret[targetX][targetY - 1] != 2)
				{
					allAction[arrayNum][0] = targetX;
					allAction[arrayNum][1] = targetY - 1;
					arrayNum += 1;
				}

				if (arrayNum == 0)
				{
					break;
				}

				gettimeofday(&seed, NULL);
				srand(seed.tv_usec);
				nowAction = rand() % arrayNum;

				targetX = allAction[nowAction][0];
				targetY = allAction[nowAction][1];
			
				value -= 1;
			}
			ret[allLoop1][allLoop2] = value;
			if (allLoop1 == 0 && allLoop2 == 0)
			{
				firstNum = ret[0][0];
			}
		}
	}

	ret[0][0] = firstNum;

	for (allLoop1 = 0; allLoop1 < allY; allLoop1++)
	{
		for (allLoop2 = 0; allLoop2 < allX; allLoop2 ++)
		{
			if (ret[allLoop1][allLoop2] == 'E')
			{
				ret[allLoop1][allLoop2] = 1;
			}
			else if (ret[allLoop1][allLoop2] == 'M')
			{
				ret[allLoop1][allLoop2] = -1 * PUNISHMENT;
			}
			else if (ret[allLoop1][allLoop2] == 'B')
			{
				ret[allLoop1][allLoop2] = 2;
			}
		}
	}

	int a, b;

	for (a = 0; a < 3; a++)
	{
		for (b = 0; b < 4; b++)
		{
			printf("%f ", ret[a][b]);
		}
		printf("\n");
	}

	return 0;
}
