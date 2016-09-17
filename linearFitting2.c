#include <stdio.h>

int main()
{
	double data[4][2] = {{1, 3}, 
			     {2, 5}, 
			     {3, 7}, 
			     {4, 9}};
	const int dataCount = sizeof(data) / sizeof(data[0]);

	double t1, t2, t3, t4;

	double theta;
	double bias;

	int count;

	for (count = 0; count < dataCount; count++)
	{
		t1 += data[count][0] * data[count][1];
		t2 += data[count][0];
		t3 += data[count][1];
		t4 += data[count][0] * data[count][0];
	}

	theta = (dataCount * t1 - t2 * t3) / (dataCount * t4 - t2 * t2);
	bias = (t4 * t3 - t2 * t1) / (dataCount * t4 - t2 * t2);

	printf("%lf %lf\n", theta, bias);

	return 0;
}
