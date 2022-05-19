#pragma once
#include <cassert>
#include <math.h>

#ifdef QUATERNIONLIBRARY_EXPORTS
#define QUATERNIONLIBRARY_API __declspec(dllexport)
#else
#define QUATERNIONLIBRARY_API __declspec(dllimport)
#endif

typedef struct Qv {
	float qx, qy, qz;

	Qv() : qx(0), qy(0), qz(0) {}
	Qv(float qw_array[3]) : qx(qw_array[0]), qy(qw_array[1]), qz(qw_array[2]) {}
	const float& operator[](int i) const {
		assert(i >= 0);
		assert(i < 3);
		switch (i) {
		case 0: return qx; break;
		case 1: return qy; break;
		case 2: return qz; break;
		default: return qx;
		}
	}
	float& operator[](int i) {
		assert(i >= 0);
		assert(i < 3);
		switch (i) {
		case 0: return qx; break;
		case 1: return qy; break;
		case 2: return qz; break;
		default: return qx;
		}
	}
} Qv_t;

typedef struct Quaternion {
	float q0;
	Qv qv;

	Quaternion() : q0(0), qv() {}

	Quaternion(float q_array[4]) : q0(q_array[0]), qv(&q_array[1]) {}
	const float& operator[](int i) const {
		assert(i >= 0);
		assert(i < 4);
		switch (i) {
		case 0: return q0; break;
		case 1: return qv.qx; break;
		case 2: return qv.qy; break;
		case 3: return qv.qz; break;
		default: return q0;
		}
	}
	float& operator[](int i) {
		assert(i >= 0);
		assert(i < 4);
		switch (i) {
		case 0: return q0; break;
		case 1: return qv.qx; break;
		case 2: return qv.qy; break;
		case 3: return qv.qz; break;
		default: return q0;
		}
	}
} Quaternion_t;

extern "C" QUATERNIONLIBRARY_API void Quaternion2AxisAngle(float q_array[4], float axis[3], float* angle);

