#include "pch.h"
#include "allanVariance.h"

void allanVarianceChartO(float* inputdata, float* outputdata, int N, int* nArray, int nLength){
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
	for (int i = 0; i < nLength && nArray[i] <= N / 2; i++) {				/* k=N/2 means minimum of 2 subgroups */
		n = nArray[i];
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
		B[i] = (float)acc / (2 * m);
	}
	for (int i = 0; i < nLength && nArray[i] <= N / 2; i++)
		outputdata[i] = B[i];
	delete[] A;
	delete[] B;
}

void allanVarianceChartNO(float* inputdata, float* outputdata, int N, int* nArray, int nLength)
{
	float* A, * B;
	float temp;
	double acc;
	int n, m, s, l;					/* space for N floats */
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
	for (int i = 0; i < nLength && nArray[i] <= N/2; i++) {				/* k=N/2 means minimum of 2 subgroups */
		n = nArray[i];
		m = N / n;                              /* number of subgroups */
		for (s = 0; s < m; s++) {               /* average subgroups */
			acc = 0.0;
			for (l = s * n; l < s * n + n; l++) {
				acc += inputdata[l]; 
			} 
			A[s] = (float)acc / n;
		}
		acc = 0; 
		m--;
		for (s = 0; s < m; s++) {				/* short term variance, use adjacent groups */
			temp = (A[s + 1] - A[s]); 
			acc += (double)temp * (double)temp;
		}
		B[i] = (float)acc / (2 * m);
	}
	for (int i = 0; i < nLength && nArray[i] <= N / 2; i++)
		outputdata[i] = B[i];
	delete [] A;
	delete [] B;
}
