#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	float *positionMax;
	float *positionMin;
	float **dataBelongsToMax;
	float **dataBelongsToMin;
	int maxLength;
	int minLength;
}coreInfo;

coreInfo *getDataCore(float data[][2], int length)
{
	const float dataLength = length;
	float positionMax[length];
	float positionMin[length];
	int dataCounter;
	int dataPosition;
	float dataBelongsToMax[length][2];
	float dataBelongsToMin[length][2];
	int DBTMCounter;		/* DBTM means DataBelongsToMax */
	int DBTMiCounter; 		/* DBTMi means DataBelongsToMin */
	int dataNowCounter;
	float tempDataX;
	float tempDataY;
	float MaxDistanceX = 0;
	float MaxDistanceY = 0;
	float MinDistanceX = 0;
	float MinDistanceY = 0;

	int allocCounter;
	coreInfo *finalResult;

	positionMax[0] = data[0][0];
	positionMax[1] = data[0][1];
	positionMin[0] = data[0][0];
	positionMin[1] = data[0][1];
	for (dataCounter = 0; dataCounter < dataLength; dataCounter++)
	{
		if (positionMax[0] < data[dataCounter][0] &&
			positionMax[1] < data[dataCounter][1])
		{
			positionMax[0] = data[dataCounter][0];
			positionMax[1] = data[dataCounter][1];
		}
		if (positionMin[0] > data[dataCounter][0] &&
			positionMin[1] > data[dataCounter][1])
		{
			positionMin[0] = data[dataCounter][0];
			positionMin[1] = data[dataCounter][1];
		}
	}

	for (dataCounter = 0; dataCounter < 4; dataCounter++)	/* Redo 4 times to check. */
	{
		DBTMCounter = 0;
		DBTMiCounter = 0;
		for (dataPosition = 0; dataPosition < dataLength; dataPosition++)
		{
			MaxDistanceX = abs(positionMax[0] - data[dataPosition][0]);
			MinDistanceX = abs(positionMin[0] - data[dataPosition][0]);
			MaxDistanceY = abs(positionMax[1] - data[dataPosition][1]);
			MinDistanceY = abs(positionMin[1] - data[dataPosition][1]);

			if ((MaxDistanceX + MaxDistanceY) / 2 < (MinDistanceX + MinDistanceY) / 2)
			{
				dataBelongsToMax[DBTMCounter][0] = data[dataPosition][0];
				dataBelongsToMax[DBTMCounter][1] = data[dataPosition][1];
				DBTMCounter += 1;
			}
			else
			{
				dataBelongsToMin[DBTMiCounter][0] = data[dataPosition][0];
				dataBelongsToMin[DBTMiCounter][1] = data[dataPosition][1];
				DBTMiCounter += 1;
			}
		}

		if (DBTMCounter != 0)
		{
			tempDataX = 0;
			tempDataY = 0;
			for (dataNowCounter = 0; dataNowCounter < DBTMCounter; dataNowCounter++)
			{
				tempDataX += dataBelongsToMax[dataNowCounter][0];
				tempDataY += dataBelongsToMax[dataNowCounter][1];
			}

			positionMax[0] = tempDataX / DBTMCounter;
			positionMax[1] = tempDataY / DBTMCounter;
		}
		if (DBTMiCounter != 0)
		{
			tempDataX = 0;
			tempDataY = 0;
			for (dataNowCounter = 0; dataNowCounter < DBTMCounter; dataNowCounter++)
			{
				tempDataX += dataBelongsToMin[dataNowCounter][0];
				tempDataY += dataBelongsToMin[dataNowCounter][1];
			}

			positionMin[0] = tempDataX / DBTMiCounter;
			positionMin[1] = tempDataY / DBTMiCounter;
		}

	}

	finalResult = (coreInfo *)calloc(sizeof(coreInfo), 1);
	if (finalResult == NULL)
	{
		return NULL;
	}

	finalResult->positionMax = (float *)calloc(sizeof(float), 1);
	if (finalResult->positionMax == NULL)
	{
		return NULL;
	}
	finalResult->positionMin = (float *)calloc(sizeof(float), 1);
	if (finalResult->positionMin == NULL)
	{
		return NULL;
	}

	finalResult->dataBelongsToMax = (float **)calloc(sizeof(float *), DBTMCounter);
	if (finalResult->dataBelongsToMax == NULL)
	{
		return NULL;
	}
	for (allocCounter = 0; allocCounter < DBTMCounter; allocCounter++)
	{
		finalResult->dataBelongsToMax[allocCounter] = (float *)calloc(sizeof(float), 2);
		if (finalResult->dataBelongsToMax[allocCounter] == NULL)
		{
			return NULL;
		}
	}

	finalResult->dataBelongsToMin = (float **)calloc(sizeof(float *), DBTMiCounter);
	if (finalResult->dataBelongsToMin == NULL)
	{
		return NULL;
	}
	for (allocCounter = 0; allocCounter < DBTMiCounter; allocCounter++)
	{
		finalResult->dataBelongsToMin[allocCounter] = (float *)calloc(sizeof(float), 2);
		if (finalResult->dataBelongsToMin[allocCounter] == NULL)
		{
			return NULL;
		}
	}

	finalResult->positionMax[0] = positionMax[0];
	finalResult->positionMax[1] = positionMax[1];
	finalResult->positionMin[0] = positionMin[0];
	finalResult->positionMin[1] = positionMin[1];

	for (dataCounter = 0; dataCounter < DBTMCounter; dataCounter++)
	{
		finalResult->dataBelongsToMax[dataCounter][0] = dataBelongsToMax[dataCounter][0];
		finalResult->dataBelongsToMax[dataCounter][1] = dataBelongsToMax[dataCounter][1];
	}
	for (dataCounter = 0; dataCounter < DBTMiCounter; dataCounter++)
	{
		finalResult->dataBelongsToMin[dataCounter][0] = dataBelongsToMin[dataCounter][0];
		finalResult->dataBelongsToMin[dataCounter][1] = dataBelongsToMin[dataCounter][1];
	}
	
	finalResult->maxLength = DBTMCounter;
	finalResult->minLength = DBTMiCounter;

	return finalResult;;
}

void freeDataCore(coreInfo *target)
{
	int freeCounter;
	
	free(target->positionMax);
	free(target->positionMin);

	for (freeCounter = 0; target->dataBelongsToMax[freeCounter] != NULL; freeCounter++)
	{
		free(target->dataBelongsToMax[freeCounter]);
	}
	for (freeCounter = 0; target->dataBelongsToMin[freeCounter] != NULL; freeCounter++)
	{
		free(target->dataBelongsToMin[freeCounter]);
	}

	free(target->dataBelongsToMax);
	free(target->dataBelongsToMin);
	
	free(target);
}

int main()
{
	coreInfo *dataCore;
	float data[][2] = {{1, 2}, {1, 3}, {3, 2}, {3, 3}};
	int dataCounter;

	dataCore = getDataCore(data, 4);

	printf("All of data:\n");
	for (dataCounter = 0; dataCounter < sizeof(data) / 8; dataCounter++)
	{
		printf("x: %f y: %f\n", data[dataCounter][0], data[dataCounter][1]);
	}
	printf("---------------\n");

	printf("First piece: x: %f y: %f\n", dataCore->positionMax[0], dataCore->positionMax[1]);
	printf("Second piece: x: %f y: %f\n", dataCore->positionMin[0], dataCore->positionMin[1]);

	printf("The data that belongs to first piece:\n");
	for (dataCounter = 0; dataCounter < dataCore->maxLength; dataCounter++)
	{
		printf("x: %f y: %f\n", dataCore->dataBelongsToMax[dataCounter][0], dataCore->dataBelongsToMax[dataCounter][1]);
	}
	printf("----------------\n");
	
	printf("The data that belongs to second piece:\n");
	for (dataCounter = 0; dataCounter < dataCore->minLength; dataCounter++)
	{
		printf("x: %f y: %f\n", dataCore->dataBelongsToMin[dataCounter][0], dataCore->dataBelongsToMin[dataCounter][1]);
	}

	freeDataCore(dataCore);

	return 0;
}
