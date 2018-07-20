#ifndef EV3_APP_LINETRACER_H_
#define EV3_APP_LINETRACER_H_

#include "LineMonitor.h"
#include "BalancingWalker.h"
#include "PidController.h"
#include "ev3api.h"
#include "SonarSensor.h"
#include "Starter.h"
#include "LineTracer.h"
#include "Calibration.h"

class LeftCourseTracer {

public:
  
    LeftCourseTracer(LineTracer lineTracer);

private:

    BehaviorHolder mBehaviorHolder;
    LeftCourseTracer mLineTracer;
    void run();
	void mortorControll(ev3api::Motor& motor, int deg, int pwm);

};

#endif