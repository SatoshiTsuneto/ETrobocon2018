#include "LineTracer.h"

/**
 * コンストラクタ
 * @param lineMonitor     ライン判定
 * @param balancingWalker 倒立走行
 */
LineTracer::LineTracer(const LineMonitor* lineMonitor,
                        const Starter* starter,
                        Calibration* calibration,
                        BalancingWalker* balancingWalker,
                        PidController* pidController,
                        ev3api::Motor& leftWheel,
                        ev3api::Motor& rightWheel,
                        ev3api::Motor& tailWheel)

     :mLineMonitor(lineMonitor),
      mStarter(starter),
      mBalancingWalker(balancingWalker),
      mPidController(mPidController),
      mLeftWheel(leftWheel),
	mRightWheel(rightWheel),
      mTailWheel(tailWheel){

      }

/**
 * デストラクタ
 */
LineTracer::~LineTracer() {
}

// 指定されたモーターにスピードと速さを指定する。
// degは目的の角度。PWMはモータの速さで、正の値で正回転、負の値で逆回転。
void LineTracer::mortorControll(ev3api::Motor& motor, int deg, int pwm) {
	motor.setPWM(pwm);
	if (deg > 0){
		while(motor.getCount() < deg);
	}else{
		while(motor.getCount() > deg);
	}
	motor.setPWM(0);
}

// // FIXME 引数が入った状態でコールバックを実行できない
// void LineTracer::execBehavior(auto callBack){
//       callBack();
// }



