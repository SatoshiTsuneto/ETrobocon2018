#ifndef EV3_APP_LINETRACER_H_
#define EV3_APP_LINETRACER_H_

#include "LineTracer.h"
#include "LineMonitor.h"
#include "BalancingWalker.h"
#include "PidController.h"
#include "ev3api.h"
#include "SonarSensor.h"


class BehaviorHolder{

private:

    LineTracer mLineTracer;
    void init();
    int calcDirection();
    void mortorControll(ev3api::Motor& motor, int deg, int pwm);

    auto initialize;
    auto calibration;
    auto taskNormal;
    auto runOnOff;
    auto startDashByBluetooth;

public:
    BehaviorHolder(LineTracer lineTracer);
    ~BehaviorHolder();

    auto findBehaviorById(short id);

    // ID
    static const short ID_INITIALIZE;
    static const short ID_CALIBRATION;
    static const short ID_TASKNORMAL;
    static const short ID_RUNONOFF;
    static const short ID_STARTDASH_BY_BLUETOOTH;

}

BehaviorHolder::BehaviorHolder(/* args */)
{
}

BehaviorHolder::~BehaviorHolder()
{
}

#endif
