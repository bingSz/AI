#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define PICTURESIZE	30000	/* 100 * 100 * 3 */

typedef struct
{
	double *theta;
	double bias;
	int len;
}SVMRet;

double vectorTimes(double *vec1, int *vec2, int len)
{
	double ret = 0;
	int count;

	for (count = 0; count < len; count++)
	{
		ret += vec1[count] * vec2[count];
	}

	return ret;
}

int getfileLine(char *path)
{
	int line = 0;
	int tempchar;
	FILE *fr = fopen(path, "r");
	
	if (fr == NULL)
	{
		return -1;
	}

	while ((tempchar = fgetc(fr)) != EOF)
	{
		if (tempchar == '\n')
		{
			line += 1;
		}
	}

	fclose(fr);

	return line;
}

int SVMTrain(SVMRet *svmret, char *listPath)
{
	FILE *fr = fopen(listPath, "r");
	FILE *dataFile;
	const int fileline = getfileLine(listPath);
	char filePath[100];
	int **learningSet;
	int tempChar;
	int count;
	int subCount;
	double errorSum;
	int deltaX;

	goto start;

mallocError:
	for (; count >= 0; count--)
	{
		free(learningSet[count]);
	}
	free(learningSet);
	return -1;

start:

	if (fr == NULL)
	{
		return -1;
	}

	learningSet = (int **)malloc(sizeof(int *) * fileline);
	if (learningSet == NULL)
	{
		return -1;
	}
	for (count = 0; count < fileline; count++)
	{
		learningSet[count] = (int *)malloc(sizeof(int) * PICTURESIZE);
		if (learningSet[count] == NULL)
		{
			goto mallocError;
		}
	}

	count = 0;
	while (fgets(filePath, sizeof(filePath), fr) != NULL)
	{
		if (filePath[strlen(filePath) - 1] == '\n')
		{
			filePath[strlen(filePath) - 1] = '\0';
		}

		dataFile = fopen(filePath, "r");
		if (dataFile == NULL)
		{
			count = fileline;
			goto mallocError;
		}

		fseek(dataFile, 54, SEEK_SET);
		subCount = 0;
		while ((tempChar = fgetc(dataFile)) != EOF)
		{
			if (tempChar == 0x00)
			{
				learningSet[count][subCount] = 1;
			}
			else
			{
				learningSet[count][subCount] = -1;
			}
			subCount += 1;
		}

		fclose(dataFile);
		count += 1;
	}

	fclose(fr);

	for (count = count - 1; count >= 0; count --)
	{
		deltaX = 1;
		for (subCount = 0; subCount < 30000; subCount++)
		{
			if (learningSet[count][subCount] == 1)
			{
				if (deltaX == 1)
				{
					deltaX = -subCount;
				}

				learningSet[count][subCount + deltaX] = 1;
				learningSet[count][subCount] = -1;
			}
		}
	}

	svmret->theta = (double *)malloc(sizeof(double) * PICTURESIZE);
	if (svmret->theta == NULL)
	{
		count = fileline - 1;
		goto mallocError;
	}
	svmret->len = PICTURESIZE;

	int subsubCount;
	svmret->bias = 0;
	for (count = 0; count < fileline * 100; count++)
	{
		subCount = count % fileline;

		errorSum = vectorTimes(svmret->theta, learningSet[subCount], svmret->len) + svmret->bias;
		if (errorSum >= 1)
		{
			continue;
		}

		for (subsubCount = 0; subsubCount < svmret->len; subsubCount++)
		{
			svmret->theta[subsubCount] += 0.1 * (1 - errorSum) * learningSet[subCount][subsubCount];
		}
		svmret->bias += errorSum;
	}

	return 0;
}

double SVMPredict(SVMRet *svmret, char *filePath)
{
	int data[svmret->len];
	int count;
	int deltaX;
	FILE *fr = fopen(filePath, "r");
	
	if (fr == NULL)
	{
		return -1;
	}

	fseek(fr, 54, SEEK_SET);

	count = 0;
	while ((data[count] = fgetc(fr)) != EOF)
	{
		if (data[count] == 0x00)
		{
			data[count] = 1;
		}
		else
		{
			data[count] = 0;
		}
		count += 1;
	}

	fclose(fr);

	deltaX = 1;
	for (count = 0; count < 30000; count++)
	{
		if (data[count] == 1)
		{
			if (deltaX == 1)
			{
				deltaX = -count;
			}
			data[count + deltaX] = 1;
			data[count] = 0;
		}
	}

	return vectorTimes(svmret->theta, data, svmret->len) + svmret->bias;
}

int main(int argc, char * argv[])
{
	if (argc < 3)
	{
		printf("Usage: %s [option] [list or file]\n", argv[0]);
		return -1;
	}
	if (0 != strcmp(argv[1], "l") && 0 != strcmp(argv[1], "i"))
	{
		printf("Options: l(learning) and i(identify)\n");
		return -1;
	}
	if (0 == strcmp(argv[1], "l") && argc < 4)
	{
		printf("Usage: %s l [file] [correct answer]\n", argv[0]);
		return -1;
	}

	SVMRet *svmret;
	int count;

	if (0 == strcmp(argv[1], "l"))
	{
		if (0 != access("data", F_OK))
		{
			mkdir("data", 0700);
		}
	
		svmret = (SVMRet *)malloc(sizeof(SVMRet));
		
		SVMTrain(svmret, argv[2]);

		if (-1 == chdir("data"))
		{
			return -1;
		}
	
		FILE *learningRet;

		learningRet = fopen(argv[3], "w+");
		if (learningRet == NULL)
		{
			return -1;
		}

		fprintf(learningRet, "%lf\n", svmret->bias);
		for (count = 0; count < svmret->len; count++)
		{
			fprintf(learningRet, "%lf\n", svmret->theta[count]);
		}

		fclose(learningRet);

		if (-1 == chdir(".."))
		{
			return -1;
		}
		
		free(svmret);
	}
	else
	{
		if (0 != access("data", F_OK))
		{
			return 0;
		}
		if (-1 == chdir("data"))
		{
			return -1;
		}

		DIR *dp;
		struct dirent *dirfile;
		FILE *dataHandle = NULL;
		char linestr[20];
		double getScore;
		double tempScore;
		char identifyRet[256];
		char targetFilePath[256];

		snprintf(targetFilePath, sizeof(targetFilePath), "../%s", argv[2]);

		dp = opendir("./");
		if (dp == NULL)
		{
			return -1;
		}

		if ((svmret = (SVMRet *)malloc(sizeof(svmret))) == NULL)
		{
			return -1;
		}
		svmret->theta = (double *)malloc(sizeof(double) * 30000);
		if (svmret->theta == NULL)
		{
			return -1;
		}
		svmret->len = 30000;

redo:
		dirfile = readdir(dp);
		if (dirfile == NULL)
		{
			return -1;
		}
		if (dirfile->d_type & DT_DIR)
		{
			goto redo;
		}
	
		dataHandle = fopen(dirfile->d_name, "r");
		if (dataHandle == NULL)
		{
			free(svmret);
			return -1;
		}
	
		fgets(linestr, sizeof(linestr), dataHandle);
		svmret->bias = atof(linestr);

		count = 0;
		while (fgets(linestr, sizeof(linestr), dataHandle) != NULL)
		{
			if (linestr[strlen(linestr) - 1] == '\n')
			{
				linestr[strlen(linestr) - 1] = '\0';
			}

			svmret->theta[count] = atof(linestr);
			count += 1;
		}

		fclose(dataHandle);
		getScore = SVMPredict(svmret, targetFilePath);
		memcpy(identifyRet, dirfile->d_name, sizeof(identifyRet));

		while ((dirfile = readdir(dp)) != NULL)
		{
			if (dirfile->d_type & DT_DIR)
			{
				continue;
			}

			dataHandle = fopen(dirfile->d_name, "r");
			if (dataHandle == NULL)
			{
				free(svmret);
				return -1;
			}
	
			fgets(linestr, sizeof(linestr), dataHandle);
			svmret->bias = atof(linestr);

			count = 0;
			while (fgets(linestr, sizeof(linestr), dataHandle) != NULL)
			{
				if (linestr[strlen(linestr) - 1] == '\n')
				{
					linestr[strlen(linestr) - 1] = '\0';
				}

				svmret->theta[count] = atof(linestr);
				count += 1;
			}

			fclose(dataHandle);
			
			tempScore = SVMPredict(svmret, targetFilePath);
			if (tempScore > getScore)
			{
				getScore = tempScore;
				memcpy(identifyRet, dirfile->d_name, sizeof(identifyRet));
			}
		}
		printf("%s\n", identifyRet);
	}

	return 0;
}
