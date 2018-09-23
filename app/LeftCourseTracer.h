#ifndef EV3_APP_LEFTCOURSETRACER_H_
#define EV3_APP_LEFTCOURSETRACER_H_

#include "BehaviorHolder.h"
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
    LineTracer *mLineTracer;
    BehaviorHolder *mBehaviorHolder;

    explicit LeftCourseTracer(LineTracer *lineTracer);
    void run();
};

#endif