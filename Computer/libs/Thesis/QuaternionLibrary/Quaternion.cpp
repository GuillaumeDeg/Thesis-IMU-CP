#include "pch.h"
#include "Quaternion.h"

void Quaternion2AxisAngle(float q_array[4], float axis[3], float* angle)
{
    Quaternion q(q_array);
    *angle = 2 * acos(q.q0);

    axis[0] = q.qv.qx / (q.q0 == 1 ? 1 : sqrt(1 - q.q0 * q.q0));
    axis[1] = q.qv.qy / (q.q0 == 1 ? 1 : sqrt(1 - q.q0 * q.q0));
    axis[2] = q.qv.qz / (q.q0 == 1 ? 1 : sqrt(1 - q.q0 * q.q0));
}