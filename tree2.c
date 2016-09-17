#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct _decisionTree
{
  int condition[2];

  struct _decisionTree *left;
  struct _decisionTree *right;
}decisionTree;

void createTree(const int dataX, const int dataY, const int rdataX, const int rdataY, int data[dataX][dataY], decisionTree *tree);

void freeTreeLeft(decisionTree *tree)
{
  if (tree->left->left == NULL)
    {
      free(tree->left);
    }
  else
    {
      freeTreeLeft(tree->left);
    }
}
void freeTreeRight(decisionTree *tree)
{
  if (tree->right->right == NULL)
    {
      free(tree->right);
    }
  else
    {
      freeTreeLeft(tree->right);
    }
}

void freeTree(decisionTree *tree)
{
  freeTreeLeft(tree->left);
  freeTreeRight(tree->right);
  free(tree);
  
}
/*
int predict(decisionTree *tree, int *data)
{
  decisionTree *temp = tree;
  
  while (tree != NULL)
    {
      if (data[tree->condition[0]] != tree->condition[1])
	{
	  tree = temp;
	  return 0;
	}
      tree = tree->next;
    }
  tree = temp;
  return 1;
}
*/
int main()
{
  int data[4][3] = {{0, 1, 1}, 
		    {0, 0, 0}, 
		    {1, 1, 1}, 
		    {1, 0, 1}};
  const int dataX = sizeof(data) / sizeof(data[0]);
  const int dataY = sizeof(data) / dataX / sizeof(int);
  decisionTree *tree = (decisionTree *)malloc(sizeof(decisionTree));
  decisionTree *head = tree;

  createTree(dataX, dataY, dataX, dataY, data, tree);
  
  freeTree(tree);
  return 0;
}
void createTree(const int dataX, const int dataY, const int rdataX, const int rdataY, int data[rdataX][rdataY], decisionTree *tree)
{
  int targetData[dataX][dataY - 1];
  int targetDataOther[dataX][dataY - 1];
  int sign[dataY - 1];

  double entropy;
  double tempGini;
  double tempEntropy;
  double tempGain;
  double retGain = -10;
  int belongsToYes;
  int belongsToNo;
  double tempNo;
  double tempYes;
  double tempNoNo;
  double tempNoYes;
  double tempYesNo;
  double tempYesYes;
  int mainCount;
  int count;
  int subCount;
  int addCount;
  int mainAddCount;
  int divisionBy;
  int condition;

  for (count = 0; count < dataX - 1; count++)
    {
      if (data[count][dataY - 1] != data[count + 1][dataY - 1])
	{
	  goto skip;
	}
    }

  tree->condition[0] = -1;
  tree->condition[1] = data[count][dataY - 1];
  
 skip:
  
  belongsToYes = 0;
  belongsToNo = 0;
  for (count = 0; count < dataX; count++)
    {
      if (data[count][dataY - 1] == 0)
	{
	  belongsToNo += 1;
	}
      else
	{
	  belongsToYes += 1;
	}
    }

  entropy = -(belongsToNo * log2(belongsToNo) + belongsToYes * log2(belongsToYes));

  memset(sign, 0, sizeof(sign));

  for (mainCount = 0; mainCount < dataY - 1; mainCount++)
    {
      tempGain = 0;
      retGain = 0;
      tempGini = 0;
      tempEntropy = 0;
      divisionBy = 0;

      for (count = 0; count < dataY - 1; count++)
	{
	  if (sign[count] == 1)
	    {
	      continue;
	    }
	  tempNo = 0;
	  tempYes = 0;
	  tempNoNo = 0;
	  tempNoYes = 0;
	  tempYesNo = 0;
	  tempYesYes = 0;
	  for (subCount = 0; subCount < dataX; subCount++)
	    {
	      if (data[subCount][count] == 0)
		{
		  if (data[subCount][dataY - 1] == 1)
		    {
		      tempNoYes += 1;
		    }
		  else
		    {
		      tempNoNo += 1;
		    }
		  tempNo += 1;
		}
	      else
		{
		  if (data[subCount][dataY - 1] == 1)
		    {
		      tempYesYes += 1;
		    }
		  else
		    {
		      tempYesNo += 1;
		    }
		  tempYes += 1;
		}
	    }
	  tempNo = tempNo / dataX;
	  tempYes = tempYes / dataX;
	  tempYesYes = tempYesYes / (tempYes + 1);
	  tempYesNo = tempYesNo / (tempYes + 1);
	  tempNoYes = tempNoYes / (tempNo + 1);
	  tempEntropy = -(tempYesYes * log2(tempYesYes + 1) + tempYesNo * log2(tempYesNo + 1));
	  tempGini = tempYesYes * tempEntropy + tempYesNo * tempEntropy;
	  tempGain = entropy - tempGini;
	  tempGain *= tempGain;
	  
	  if (tempGain > retGain)
	    {
	      retGain = tempGain;
	      divisionBy = count;
	      if (tempNoYes > tempYesYes)
		{
		  condition = 0;
		}
	      else
		{
		  condition = 1;
		}
	    }
	}

      sign[divisionBy] = 1;

      tree->condition[0] = divisionBy;
      tree->condition[1] = condition;
      tree->left = (decisionTree *)malloc(sizeof(decisionTree));
      tree->left->left = NULL;
      tree->left->right = NULL;
      tree->right = (decisionTree *)malloc(sizeof(decisionTree));
      tree->right->left = NULL;
      tree->right->right = NULL;

      memset(targetData, 0, sizeof(targetData));
      memset(targetDataOther, 0, sizeof(targetDataOther));
      
      for (count = 0, mainAddCount = 0; count < dataX; count++)
	{
	  if (data[count][divisionBy] != condition)
	    {
	      continue;
	    }
	  for (subCount = 0, addCount = 0; subCount < dataY; subCount++)
	    {
	      if (subCount == divisionBy)
		{
		  continue;
		}

	      printf("|%d|%d\n", subCount, dataY);

	      targetData[mainAddCount][addCount] = data[count][subCount];
	      addCount += 1;
	      
	    }
	  mainAddCount += 1;
	}

      if (mainAddCount == 0 || addCount == 2)
	{
	  return;
	}
      
      createTree(mainAddCount, addCount, dataX, dataY - 1, targetData, tree->left);
      
      for (count = 0; count < dataX; count++)
	{
	  for (subCount = 0, addCount = 0; subCount < dataY; subCount++)
	    {
	      if (subCount == divisionBy)
		{
		  continue;
		}

	      if (data[count][divisionBy] != condition)
		{
		  targetDataOther[count][addCount] = data[count][subCount];
		  addCount += 1;
		}
	      
	    }
	}
      createTree(dataX, addCount, dataX, dataY - 1, targetDataOther, tree->right);
    }
}

