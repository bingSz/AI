#include <stdio.h>
#include <string.h>

double vectorTimes(double a[], double b[], const int dataSize)
{
  double ret;
  int count;

  ret = 0;
  for (count = 0; count < dataSize; count++)
    {
      ret += a[count] * b[count];
    }
  
  return ret;
}

int main()
{
  double data[4][2] = {{1, 1},
		       {1, 2},
		       {2, 1},
		       {2, 2}};
  int ret[4] = {-1, -1, 1, 1};

  double learningRate = 0.01;
  double theta[2];
  double errorSum;
  double bias;
  
  int count;
  int num;

  memset(theta, 0, sizeof(theta));
  for (count = 0; count < 10000; count++)
    {
      num = count % 4;

      errorSum = ret[num] - vectorTimes(theta, data[num], 2) - bias;

      if (errorSum <= 0 && ret[num] == 1)
	{
	  continue;
	}
      if (errorSum >= 0 && ret[num] == -1)
	{
	  continue;
	}

      theta[0] += errorSum * learningRate * data[num][0];
      theta[1] += errorSum * learningRate * data[num][1];
      bias += errorSum;
    }

  printf("%lf %lf %lf\n", theta[0], theta[1], bias);
  
  return 0;
}
