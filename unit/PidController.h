#ifndef EV3_UNIT_PIDCONTROLLER_H_
#define EV3_UNIT_PIDCONTROLLER_H_

#include "ev3api.h"


class PidController
{
public:
    PidController();
    virtual ~PidController();

    /**
     * PID�����Turn�l��Z�o����
     */
    float calControlledVariable(int8_t deviation);
    void setPidFactor(float kp, float ki, float kd);
    void setEdge(int edge);

private:
    int32_t mDeviation;
    int32_t mIntegral;

    float KPID_P_FACTOR;
    float KPID_I_FACTOR;
    float KPID_D_FACTOR;

    int mEdge;
};

#endif  // EV3_UNIT_PIDCONTROLLER_H_
