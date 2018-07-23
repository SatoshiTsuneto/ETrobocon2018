#ifndef EV3_APP_LINETRACER_H_
#define EV3_APP_LINETRACER_H_

#include "LineTracer.h"
#include "LineMonitor.h"
#include "BalancingWalker.h"
#include "PidController.h"
#include "ev3api.h"
#include "SonarSensor.h"

#include <functional>
#include <map>

class BehaviorHolder{

public:
    BehaviorHolder(LineTracer lineTracer);
    ~BehaviorHolder();

    enum BehaviorId{
         ID_INITIALIZE,
         ID_CALIBRATION,
         ID_NORMAL_RUN,
         ID_RUNONOFF,
         ID_STARTDASH_BY_BLUETOOTH
    };

    std::function<void(void)> findBehaviorById(BehaviorId id);

 private:

    LineTracer mLineTracer;
    map<BehaviorId,std::function<void(void)>> behavior_map;
    
    void init();
    int calcDirection();
    void mortorControll(ev3api::Motor& motor, int deg, int pwm);

};

// BehaviorHolder::BehaviorHolder(/* args */)
// {
// }

// BehaviorHolder::~BehaviorHolder()
// {
// }

#endif
