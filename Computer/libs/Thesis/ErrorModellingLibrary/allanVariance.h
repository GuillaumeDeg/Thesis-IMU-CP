#pragma once

#ifdef ERRORMODELLIBRARY_EXPORTS
#define ERRORMODELLIBRARY_API __declspec(dllexport)
#else
#define ERRORMODELLIBRARY_API __declspec(dllimport)
#endif


//Calculates the Overlapping Allan Variance for an array
//inputdata: ptr to array of floats as input
//N: size of the input array
//nArray: array of all values of n to be calculated
//outputdata: ptr to array of floats as output with length nLength
extern "C" ERRORMODELLIBRARY_API void allanVarianceChartO(float* inputdata, float* outputdata, int N, int* nArray, int nLength);

//Calculates the Non-Overlapping Allan Variance for an array
//inputdata: ptr to array of floats as input
//N: size of the input array
//nArray: array of all values of n to be calculated
//outputdata: ptr to array of floats as output with length nLength
extern "C" ERRORMODELLIBRARY_API void allanVarianceChartNO(float* inputdata, float* outputdata, int N, int* nArray, int nLength);
