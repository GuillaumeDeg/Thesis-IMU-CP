#pragma once

#ifdef ERRORMODELLIBRARY_EXPORTS
#define ERRORMODELLIBRARY_API __declspec(dllexport)
#else
#define ERRORMODELLIBRARY_API __declspec(dllimport)
#endif


//Calculates the Overlapping Allan Variance for an array
//inputdata: ptr to array of floats as input
//N: size of the input array
//outputdata: ptr to array of floats as output with length N-1
extern "C" ERRORMODELLIBRARY_API void allanVarianceChartO(float* inputdata, float* outputdata, int N);
extern "C" ERRORMODELLIBRARY_API void allanVarianceChartNO(float* inputdata, float* outputdata, int N);
