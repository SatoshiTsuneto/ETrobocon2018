#include "LeftCourseTracer.h"
#include "BehaviorHolder.h"
#include "LineTracer.h"

#include "LineMonitor.h"
#include "BalancingWalker.h"
#include "PidController.h"
#include "ev3api.h"
#include "SonarSensor.h"
#include "Starter.h"
#include "Calibration.h"

#include <functional>

using namespace std;

const float PID_1[3] = {0.3, 0.01, 0.04};
const float PID_2[3] = {1.3, 0.7, 0.1};

const short SPEED_HIGH = 100;
const short SPEED_MIDDLE = 55;
const short SPEED_LOW = 30;

LeftCourseTracer::LeftCourseTracer(LineTracer *lineTracer) : mLineTracer(lineTracer),
                                                             mBehaviorHolder(new BehaviorHolder(lineTracer)) {}

// TODO :
// 走行
void LeftCourseTracer::run() {
    static int status = 0;
    std::function<void(void)> behavior;
    switch (status) {
        //走行を開始する
        // 第一直線
        case 0:
            behavior = mBehaviorHolder->findBehaviorById(BehaviorHolder::ID_INITIALIZE);
            if (mLineTracer->mLeftWheel.getCount() > 2000) { status = 1; }
            break;

        case 1:
            behavior = mBehaviorHolder->findBehaviorById(BehaviorHolder::ID_CALIBRATION);
            break;

        case 2:
            behavior = mBehaviorHolder->findBehaviorById(BehaviorHolder::ID_NORMAL_RUN);
            mLineTracer->set_pid(const_cast<float *>(PID_1));
            mLineTracer->set_speed(SPEED_MIDDLE);
            break;

        default:
            break;
    }
    mLineTracer->exec_behavior(behavior);
}



