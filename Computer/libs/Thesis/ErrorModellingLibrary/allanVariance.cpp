#include "pch.h"
#include "allanVariance.h"

void allanVarianceChartO(float* inputdata, float* outputdata, int N)
{
	float* A, * B;
	float temp;
	double acc;
	int n, m, s, l;
	A = new float[N * 4];						/* space for N floats */
	if (A == NULL) {
		throw ExceptionNestedException;
		return;
	}
	B = new float[N * 4];						/* space for N floats */
	if (B == NULL) {
		throw ExceptionNestedException;
		return;
	}
	for (n = 1; n <= N / 2; n++) {				/* n=N/2 means minimum of 2 subgroups */
		m = N - n + 1;                          /* number of subgroups */
		for (s = 0; s < m; s++) {               /* average subgroups */
			acc = 0.0;
			for (l = s; l < s + n; l++) {
				acc += inputdata[l];
			}
			A[s] = (float)acc / n;
		}
		acc = 0;
		for (s = 0; s < m-n; s++) {				/* short term variance, use adjacent groups */
			temp = (A[s + n] - A[s]);
			acc += (double)temp * (double)temp;
		}
		B[n - 1] = (float)acc / (2 * m);
	}
	for (n = 0; n < N / 2; n++)
		outputdata[n] = B[n];
	delete[] A;
	delete[] B;
}

void allanVarianceChartNO(float* inputdata, float* outputdata, int N)
{
	float* A, * B;
	float temp;
	double acc;
	int k, m, s, l;
	A = new float[N * 4];						/* space for N floats */
	if (A == NULL) {
		throw ExceptionNestedException;
		return;
	}
	B = new float[N * 4];						/* space for N floats */
	if (B == NULL) {
		throw ExceptionNestedException;
		return;
	}
	for (k = 1; k <= N / 2; k++) {				/* k=N/2 means minimum of 2 subgroups */
		m = N / k;                              /* number of subgroups */
		for (s = 0; s < m; s++) {               /* average subgroups */
			acc = 0.0;
			for (l = s * k; l < s * k + k; l++) {
				acc += inputdata[l]; 
			} 
			A[s] = (float)acc / k;
		}
		acc = 0; 
		m--;
		for (s = 0; s < m; s++) {				/* short term variance, use adjacent groups */
			temp = (A[s + 1] - A[s]); 
			acc += (double)temp* (double)temp;
		}
		B[k - 1] = (float)acc / (2 * m);
	}
	for (k = 0; k < N / 2; k++) 
		outputdata[k] = B[k];
	delete [] A;
	delete [] B;
}
