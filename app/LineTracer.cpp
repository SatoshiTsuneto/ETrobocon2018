#include "LineTracer.h"
#include "LineMonitor.h"
#include "BalancingWalker.h"
#include "PidController.h"
#include "ev3api.h"
#include "SonarSensor.h"
#include "Starter.h"
#include "Calibration.h"
#include <functional>

/**
 * コンストラクタ
 * @param lineMonitor     ライン判定
 * @param balancingWalker 倒立走行
 */
LineTracer::LineTracer(const LineMonitor *lineMonitor,
                       const Starter *starter,
                       Calibration *calibration,
                       BalancingWalker *balancingWalker,
                       PidController *mPidController,
                       ev3api::Motor &leftWheel,
                       ev3api::Motor &rightWheel,
                       ev3api::Motor &tailWheel)
        : mLineMonitor(lineMonitor),
          mStarter(starter),
          mBalancingWalker(balancingWalker),
          mPidController(mPidController),
          mLeftWheel(leftWheel),
          mRightWheel(rightWheel),
          mTailWheel(tailWheel) {}

/**
 * デストラクタ
 */
LineTracer::~LineTracer() {}

// 指定されたモーターにスピードと速さを指定する。
// degは目的の角度。PWMはモータの速さで、正の値で正回転、負の値で逆回転。
void LineTracer::mortorControll(ev3api::Motor &motor, int deg, int pwm) {
    motor.setPWM(pwm);
    if (deg > 0) {
        while (motor.getCount() < deg);
    } else {
        while (motor.getCount() > deg);
    }
    motor.setPWM(0);
}

void LineTracer::exec_behavior(std::function<void(void)> callBack) { callBack(); }

// getter setter
void LineTracer::set_speed(short speed) { m_speed = speed; }

short LineTracer::get_spped() { return m_speed; }

void LineTracer::set_pid(float *pid) {
    for (int i = 0; i < PID_BUF; i++) {
        m_pid[i] = pid[i];
    }
}

float *LineTracer::get_pid() { return m_pid; }


