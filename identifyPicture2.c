#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>


typedef struct
{
	long int dataSize;
	long int headerSize;
}PteInfo;

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

PteInfo getbmpinfo(char *path)
{
	PteInfo pteinfo;
	long int allSize;
	long int dataSize;
	long int headerSize;

	unsigned char tempData[9] = "";
	char tempChar[5] = "";
	char sizeData[10] = "";
	int count;
	int subCount;
	FILE *fr = fopen(path, "r");

	if (fr == NULL)
	{
		pteinfo.headerSize = 0;
		pteinfo.dataSize = 0;
		return pteinfo; 
	}

	/* Read the size of file */
	fseek(fr, 2, SEEK_SET);

	fread(tempData, 4, 1, fr);

	subCount = 0;
	for (count = strlen(tempData) - 1; count >= 0; count --)
	{
		snprintf(tempChar, sizeof(tempChar), "%x", (int)tempData[count]);
		strncat(sizeData, tempChar, sizeof(sizeData) - strlen(sizeData));
		subCount += 1;
	}

	allSize = strtol(sizeData, NULL, 16);

	/* Read the size of header */
	fseek(fr, 10, SEEK_SET);

	fread(tempData, 4, 1, fr);

	memset(sizeData, 0, sizeof(sizeData));

	subCount = 0;
	for (count = strlen(tempData) - 1; count >= 0; count --)
	{
		snprintf(tempChar, sizeof(tempChar), "%x", (int)tempData[count]);
		strncat(sizeData, tempChar, sizeof(sizeData) - strlen(sizeData));
		subCount += 1;
	}

	headerSize = strtol(sizeData, NULL, 16);

	pteinfo.headerSize = headerSize;
	pteinfo.dataSize = allSize - headerSize;

	fclose(fr);

	return pteinfo;
}

int PteSVMTrain(SVMRet *svmret, char *listPath, int key)
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

	long int PICTURESIZE;
	int skipSize;

	char firstfilepath[255] = "";
	PteInfo pteinfo;

	fgets(firstfilepath, sizeof(firstfilepath), fr);
	if (firstfilepath[strlen(firstfilepath) - 1] == '\n')
	{
		firstfilepath[strlen(firstfilepath) - 1] = '\0';
	}

	fseek(fr, 0, SEEK_SET);

	pteinfo = getbmpinfo(firstfilepath);

	PICTURESIZE = pteinfo.dataSize;
	skipSize = (int)pteinfo.headerSize;

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

		fseek(dataFile, skipSize, SEEK_SET);
		subCount = 0;
		while ((tempChar = fgetc(dataFile)) != EOF)
		{
			if (tempChar == key)
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
		for (subCount = 0; subCount < PICTURESIZE; subCount++)
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

double PteSVMPredict(SVMRet *svmret, char *filePath, int key)
{
	int *data;
	int count;
	int deltaX;
	FILE *fr = fopen(filePath, "r");

	data = (int *)malloc(sizeof(int) * svmret->len);

	long int PICTURESIZE;
	int skipSize;
	PteInfo pteinfo;

	pteinfo = getbmpinfo(filePath);
	PICTURESIZE = pteinfo.dataSize;
	skipSize = pteinfo.headerSize;

	if (fr == NULL)
	{
		return -1;
	}

	fseek(fr, skipSize, SEEK_SET);

	count = 0;
	while ((data[count] = fgetc(fr)) != EOF)
	{
		if (data[count] == key)
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
	for (count = 0; count < PICTURESIZE; count++)
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
		
		PteSVMTrain(svmret, argv[2], 0x00);

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
		long int PICTURESIZE;
		PteInfo pteinfo;

		pteinfo = getbmpinfo(argv[2]);
		PICTURESIZE = pteinfo.dataSize;
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
		svmret->theta = (double *)malloc(sizeof(double) * PICTURESIZE);
		if (svmret->theta == NULL)
		{
			return -1;
		}
		svmret->len = PICTURESIZE;

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
		getScore = PteSVMPredict(svmret, targetFilePath, 0x00);
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
			
			tempScore = PteSVMPredict(svmret, targetFilePath, 0x00);
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
