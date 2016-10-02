#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <signal.h>
#include <math.h>

typedef struct
{
	long int dataSize;
	long int headerSize;
}PteInfo;

typedef struct
{
	int x;
	int y;
}PTE;

int learn(const char *path, const char *correctRet, int key, const int x, const int y);
char *identify(const char *path, int key, int x, int y);
char *compare(PTE *pteInInfo, int pteInInfoSize);
float compareData(PTE *pteInInfo, int pteInInfoSize);
float computeProbability(PTE *pteDataInfo, PTE *pteInInfo, int size, int allProbability);

int main(int argc, char * argv[])
{
	char *ret = NULL;

	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s [option] [picture path] [correct result].\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (0 == strcmp(argv[1], "l"))
	{
		if (argc < 4)
		{
			fprintf(stderr, "Usage: %s l [picture path] [correct result].\n", argv[0]);
			exit(EXIT_FAILURE);
		}
		if (-1 == learn(argv[2], argv[3], 0x00, 200, 200))
		{
			fprintf(stderr, "Learning failure.\n");
			exit(EXIT_FAILURE);
		}
	}
	else if (0 == strcmp(argv[1], "i"))
	{
		if (argc < 3)
		{
			fprintf(stderr, "Usage: %s i [picture path].\n", argv[0]);
			exit(EXIT_FAILURE);
		}
		if (NULL == (ret = identify(argv[2], 0x00, 200, 200)))
		{
			exit(EXIT_FAILURE);
		}
		fprintf(stdout, "%s\n", ret);
	}
	else
	{
		fprintf(stderr, "Unknown command: %s\n", argv[1]);
		fprintf(stderr, "l(learning) or i(identification)\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}

PteInfo getbmpinfo(const char *path)
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
	for (count = strlen((const char *)tempData) - 1; count >= 0; count --)
	{
		snprintf(tempChar, sizeof(tempChar), "%x", (int)tempData[count]);
		strncat(sizeData, tempChar, sizeof(sizeData) - strlen(sizeData) - 1);
		subCount += 1;
	}

	allSize = strtol(sizeData, NULL, 16);

	/* Read the size of header */
	fseek(fr, 10, SEEK_SET);

	fread(tempData, 4, 1, fr);

	memset(sizeData, 0, sizeof(sizeData));

	subCount = 0;
	for (count = strlen((const char *)tempData) - 1; count >= 0; count --)
	{
		snprintf(tempChar, sizeof(tempChar), "%x", (int)tempData[count]);
		strncat(sizeData, tempChar, sizeof(sizeData) - strlen(sizeData) - 1);
		subCount += 1;
	}

	headerSize = strtol(sizeData, NULL, 16);

	pteinfo.headerSize = headerSize;
	pteinfo.dataSize = allSize - headerSize;

	fclose(fr);

	return pteinfo;
}

int learn(const char *path, const char *correctRet, int key, const int x, const int y)
{
	int pteX, pteY;
	int pteDeltaX, pteDeltaY;
	int i;
	const int pteXYSize = 3 * x * y;
	int *pteXY;
	int addrCount = 0;

	int readChar;
	int fileCount = 0;
	char SfileCount[100] = "";
	char outFolder[1000] = "";
	FILE *inH = NULL;
	FILE *outH = NULL;

	PteInfo pteinfo;
	pteinfo = getbmpinfo(path);

	pteXY = (int *)malloc(sizeof(int) * pteXYSize);

	if (0 != access("data", F_OK))
	{
		if (-1 == mkdir("data", 0755))
		{
			return -1;
		}
	}
	snprintf(outFolder, sizeof(outFolder), "data/%s", correctRet);
	if (0 != access(outFolder, F_OK))
	{
		if (-1 == mkdir(outFolder, 0755))
		{
			return -1;
		}
	}

	for (;;)
	{
		snprintf(SfileCount, sizeof(SfileCount), "%s/%d", outFolder, fileCount);
		if (0 != access(SfileCount, F_OK))
		{
			snprintf(SfileCount, sizeof(SfileCount), "%d", fileCount);
			break;
		}
		fileCount += 1;
	}

	inH = fopen(path, "r");
	if (inH == NULL)
	{
		return -1;
	}
	fseek(inH, pteinfo.headerSize, SEEK_SET);		/* skip file header */

	if (-1 == chdir("data"))
	{
		return -1;
	}
	if (-1 == chdir(correctRet))
	{
		return -1;
	}

	outH = fopen(SfileCount, "w+");
	if (outH == NULL)
	{
		return -1;
	}

	i = 0;
	while ((readChar = fgetc(inH)) != EOF)
	{
		addrCount += 1;
		if (readChar != key)
		{
			continue;
		}

		pteXY[i] = addrCount;
		i += 1;
	}
	pteXY[i] = -1;

	pteDeltaY = -(pteXY[0] % (3 * x) + 1);
	pteDeltaX = -(pteXY[0] / (3 * x) + 1);

	for (i = 0; pteXY[i] != -1; i++)
	{
		pteX = pteXY[i] / (3 * x) + 1;
		pteY = pteXY[i] % (3 * x) + 1;
/*		if (pteY == pteXY[i + 1] / (3 * x) && pteY == pteXY[i + 2] / (3 * x))
		{
			i += 2;
		}
		pteX += 1;
		pteX = pteX / 3 + 1;
		pteY = y - pteY;
*/
		pteX += pteDeltaX;
		pteY += pteDeltaY;

		if (0 >= fprintf(outH, "%d %d\n", pteX, pteY))
		{
			return -1;
		}
	}

	fclose(outH);
	fclose(inH);

	free(pteXY);

	return 0;
}

char *identify(const char *path, int key, int x, int y)
{
	int everyChar;
	FILE *inH = NULL;

	int i;
	int pteDeltaX, pteDeltaY;
	int pteX, pteY;
	int *pteXY;
	int pteAddrCount = 0;
	int addrCount = 0;
	int pteInInfoCount = 0;
	char ret[257] = "";
	char *retP;
	PTE *pteInInfo;

	PteInfo pteinfo;
	pteinfo = getbmpinfo(path);

	pteXY = (int *)calloc(sizeof(int), x * y * 3);
	if (pteXY == NULL)
	{
		return NULL;
	}
	pteInInfo = (PTE *)calloc(sizeof(PTE), x * y * 3);
	if (pteInInfo == NULL)
	{
		return NULL;
	}

	inH = fopen(path, "r");
	if (inH == NULL)
	{
		return NULL;
	}
	fseek(inH, pteinfo.headerSize, SEEK_SET);		/* skip file header */

	if (-1 == chdir("data"))
	{
		return NULL;
	}

	while ((everyChar = fgetc(inH)) != EOF)
	{
		addrCount += 1;
		if (everyChar != key)
		{
			continue;
		}

		pteXY[pteAddrCount] = addrCount;
		pteAddrCount += 1;
	}
	pteXY[pteAddrCount] = -1;

	pteDeltaY = -(pteXY[0] % (3 * x) + 1);
	pteDeltaX = -(pteXY[0] / (3 * x) + 1);

	for (i = 0; pteXY[i] != -1; i++)
	{
		pteX = pteXY[i] / (3 * x) + 1;
		pteY = pteXY[i] % (3 * x) + 1;
/*		if (pteY == pteXY[i + 1] / (3 * x) && pteY == pteXY[i + 2] / (3 * x))
		{
			i += 2;
		}
		pteX += 1;
		pteX = pteX / 3 + 1;
		pteY = y - pteY;
*/
		pteX += pteDeltaX;
		pteY += pteDeltaY;

		pteInInfo[pteInInfoCount].x = pteX;
		pteInInfo[pteInInfoCount].y = pteY;
		pteInInfoCount += 1;
	}
	
	fclose(inH);

	free(pteXY);

	return compare(pteInInfo, pteInInfoCount);
}

char *compare(PTE *pteInInfo, int pteInInfoSize)
{
	float highest = -1000.000, tempHighest = -1000.000;
	float secHighest = -1000, thiHighest = -1000;
	char *highestName = NULL;
	static char tempHighestName[257] = "";
	static char tempSecHighestName[257] = "";
	static char tempThiHighestName[257] = "";

	struct dirent *dirInfo;
	DIR *dp;

	dp = opendir(getcwd(NULL, 1024));
	if (dp == NULL)
	{
		return NULL;
	}

	while ((dirInfo = readdir(dp)) != NULL)
	{
		if (0 == strcmp(dirInfo->d_name, ".") ||
		    0 == strcmp(dirInfo->d_name, ".."))
		{
			continue;
		}

		if (dirInfo->d_type & DT_DIR)
		{
			if (-1 == chdir(dirInfo->d_name))
			{
				return NULL;
			}
			if (highest < (tempHighest = compareData(pteInInfo, pteInInfoSize)))
			{
				highest = tempHighest;
				memcpy(tempHighestName, dirInfo->d_name, sizeof(tempHighestName));
			}
			if (highest > tempHighest && tempHighest > secHighest)
			{
				secHighest = tempHighest;
				memcpy(tempSecHighestName, dirInfo->d_name, sizeof(tempSecHighestName));
			}
			if (secHighest > tempHighest && tempHighest > thiHighest)
			{
				thiHighest = tempHighest;
				memcpy(tempThiHighestName, dirInfo->d_name, sizeof(tempThiHighestName));
			}

			if (-1 == chdir("../"))
			{
				return NULL;
			}
		}
	}

	closedir(dp);

	if (0 == strcmp(tempHighestName, tempSecHighestName) || 0 == strcmp(tempHighestName, tempThiHighestName))
	{
		highestName = tempHighestName;
	}
	else if (0 == strcmp(tempSecHighestName, tempThiHighestName) && strlen(tempSecHighestName) != 0)
	{
		highestName = tempSecHighestName;
	}
	else
	{
		highestName = tempHighestName;
	}
	
	free(pteInInfo);

	return highestName;
}

float compareData(PTE *pteInInfo, int pteInInfoSize)
{
	PTE pteDataInfo[30000];
	int pteDataInfoCount;
	int pteX, pteY;
	float probability = -1000.000, tempProbability = -1000.000;

	FILE *dataH = NULL;
	char lineString[1024] = "";

	DIR *dp;
	struct dirent *dirInfo;

	dp = opendir("./");

	while ((dirInfo = readdir(dp)) != NULL)
	{
		if (0 == strcmp(dirInfo->d_name, ".") ||
			0 == strcmp(dirInfo->d_name, ".."))
		{
			continue;
		}
		if (dirInfo->d_type & DT_DIR)
		{
			continue;
		}

		if ((dataH = fopen(dirInfo->d_name, "r")) == NULL)
		{
			fprintf(stderr, "Open file: %s, failure.\n", dirInfo->d_name);
			exit(EXIT_FAILURE);
		}

		pteDataInfoCount = 0;
		while (fgets(lineString, sizeof(lineString), dataH) != NULL)
		{
			if (lineString[strlen(lineString)-1] == '\n')
			{
				lineString[strlen(lineString)-1] = '\0';
			}
			if (2 > sscanf(lineString, "%d %d", &pteX, &pteY))
			{
				return -1;
			}

			pteDataInfo[pteDataInfoCount].x = pteX;
			pteDataInfo[pteDataInfoCount].y = pteY;
			pteDataInfoCount += 1;
		}

		fclose(dataH);

		if (probability < (tempProbability = computeProbability(pteDataInfo, pteInInfo, pteDataInfoCount, pteInInfoSize)))
		{
			probability = tempProbability;
		}

	}

	closedir(dp);

	return probability;
}

float computeProbability(PTE *pteDataInfo, PTE *pteInInfo, int dataSize, int allProbability)
{
	int i;
	int size = (size > allProbability)?size:allProbability;
	int IhighestX = 0, IhighestY = 0;
	int DhighestX = 0, DhighestY = 0;
	float total = 0;

	for (i = 0; i < size; i++)
	{
		if (IhighestX < pteInInfo[i].x)
		{
			IhighestX = pteInInfo[i].x;
		}
		if (IhighestY < pteInInfo[i].y)
		{
			IhighestY = pteInInfo[i].y;
		}
		if (DhighestX < pteDataInfo[i].x)
		{
			DhighestX = pteDataInfo[i].x;
		}
		if (DhighestY < pteDataInfo[i].y)
		{
			DhighestY = pteDataInfo[i].y;
		}
		
/*		if (abs(pteDataInfo[i].x - pteInInfo[i].x) <= 1 &&
			abs(pteDataInfo[i].y - pteInInfo[i].y) <= 1)
		{
			total += 3;
		}
		else if (abs(pteDataInfo[i].x - pteInInfo[i].x) <= 2 &&
			abs(pteDataInfo[i].y - pteInInfo[i].y) <= 2)
		{
			total += 2;
		}
		else if (abs(pteDataInfo[i].x - pteInInfo[i].x) <= 3 &&
			abs(pteDataInfo[i].y - pteInInfo[i].y) <= 3)
		{
			total += 1;
		}
*/	
		total += exp(-((pteDataInfo[i].x - pteInInfo[i].x) * (pteDataInfo[i].x - pteInInfo[i].x) + (pteDataInfo[i].y - pteInInfo[i].y) * (pteDataInfo[i].y - pteInInfo[i].y)));
	}

	return total - abs(IhighestX - DhighestX) - abs(IhighestY - DhighestY) - abs(dataSize - allProbability);
}
