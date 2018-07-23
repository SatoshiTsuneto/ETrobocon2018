#ifndef EV3_APP_LINETRACER_H_
#define EV3_APP_LINETRACER_H_

#include "LineMonitor.h"
#include "BalancingWalker.h"
#include "PidController.h"
#include "ev3api.h"
#include "SonarSensor.h"
#include "Starter.h"
#include "Calibration.h"

#include <functional>

// 走行体の基底クラス
class LineTracer {

public:

    LineTracer(const LineMonitor* lineMonitor,
               const Starter* starter,
               Calibration* calibration,
               BalancingWalker* balancingWalker,
               PidController* pidController,
			   ev3api::Motor& leftWheel,
			   ev3api::Motor& rightWheel,
               ev3api::Motor& tailWheel);
               
    virtual ~LineTracer();


    const LineMonitor* mLineMonitor;
    const Starter* mStarter;
    Calibration* mCalibration;
    BalancingWalker* mBalancingWalker;
    PidController* mPidController;
	ev3api::Motor mLeftWheel;
	ev3api::Motor mRightWheel;
    ev3api::Motor mTailWheel;

    void mortorControll(ev3api::Motor& motor, int deg, int pwm);

    void exec_behavior(std::function<void(void)> callBack);

    void set_speed(short spped);
    short get_spped();

    void set_pid(float pid[3]);
    float* get_pid(); 

private:

    short m_speed;
    float m_pid[3];
   
};

#endif

