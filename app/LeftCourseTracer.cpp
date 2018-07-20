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

#include <map>
using namespace std;

const float PID_1[3] = {0.3, 0.01, 0.04};
const float PID_2[3] = {1.3, 0.7, 0.1};

const short SPEED_HIGH = 100;
const short SPEED_MIDDLE = 55;
const short SPEED_LOW = 30;



LeftCourseTracer::LeftCourseTracer(LineTracer lineTracer){
	mLineTracer = lineTracer;
	mBehaviorHolder = new BehaviorHolder(mLineTracer);
}


// TODO : 
// 走行
void LeftCourseTracer::run() {
	static int status = 0;
	auto func;
	switch(status) {
		//走行を開始する
		// 第一直線
		case 0:
			func = mBehaviorHolder.findBehaviorById(ID_INITIALIZE);
			if (mLineTracer.mLeftWheel.getCount() > 2000) { status = 1; }
			break;

		case 1:
			func = mBehaviorHolder.findBehaviorById(ID_CALIBRATION);
			break;

		case 2:
			func = mBehaviorHolder.findBehaviorById(ID_TASKNORMAL);
			func(PID_2,SPEED_MIDDLE);
			break;
	}
}
