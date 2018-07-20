#include "BehaviorHolder.h"
#include "LineTracer.h"
#include "LineMonitor.h"
#include "BalancingWalker.h"
#include "PidController.h"
#include "ev3api.h"
#include "SonarSensor.h"

// TODO:マップでIDとラムダオブジェクトを紐づけて管理する。
static const short BehaviorHolder::ID_INITIALIZE = 0;
static const short BehaviorHolder::ID_CALIBRATION = 1;
static const short BehaviorHolder::ID_STARTDASH_BY_BLUETOOTH = 2;
static const short BehaviorHolder::ID_TASKNORMAL = 3;
static const short BehaviorHolder::ID_RUNONOFF = 4;

//コンストラクタ
BehaviorHolder::BehaviorHolder(LineTracer lineTracer){
    mLineTracer = lineTracer;
    init();
}


auto BehaviorHolder::findBehaviorById(short id){
    auto result;
    switch(id) {

        case ID_INITIALIZE: 
        result = initialize; 
        break;

        case ID_CALIBRATION: 
        result = calibration; 
        break;

        case ID_STARTDASH_BY_BLUETOOTH: 
        result = startDashByBluetooth; 
        break;

        case ID_TASKNORMAL: 
        result = taskNormal; 
        break;

        case ID_RUNONOFF: 
        result = runOnOff; 
        break;
    
    default :
        // TODO:例外処理したい
        break;
    } 
    return result;
}


/**
 * 走行体の向きを計算する
 * @param isOnLine true:ライン上/false:ライン外
 * @retval 30  ライン上にある場合(右旋回指示)
 * @retval -30 ライン外にある場合(左旋回指示)
 */
int BehaviorHolder::calcDirection() {
	return mLineTracer.mPidController->calControlledVariable(mLineTracer.mLineMonitor->getDeviation());
}

// ラムダ式の実装
void BehaviorHolder::init(){

     // 初期化
    initialize = []{
    	mLineTracer.mTailWheel.reset();
        mLineTracer.mTailWheel.setCount(0);
        mortorControll(mLineTracer.mTailWheel,85,50); //テイル降ろす
        mLineTracer.mCalibration->init();
    };

    // キャリブレーション
    calibration = []{

        // TODO : きったねえので要修正
        bool result = false;

        result = mLineTracer.mCalibration->calibrateGyro(1);
        result = mLineTracer.mCalibration->calibrateBlack(1);
        result = mLineTracer.mCalibration->calibrateWhite(mLineTracer.mStarter->isPushed());
        result = mLineTracer.mCalibration->calibrationColor(mLineTracer.mStarter->isPushed());

        if (result){
            while(mLineTracer.mStarter->isPushed());
        }
    };

    // スタートダッシュ
    startDashByBluetooth = [](int* mBt_data){
        if (mLineTracer.mStarter->isPushed() || *mBt_data == '1') {//ボタン押すＯＲブルーツースから１送るとスタート
            mLineTracer.mCalibration->calibrateLineThreshold();
            mLineTracer.mortorControll(mTailWheel,100,50);
            // tailモータの押し出す力によって、左右ホイールが3°動いたら次のStateに移る
            mLineTracer.mTailWheel.setPWM(-50);
        }
    };


    // 直線走行
    taskNormal = [](const float *pidSet,const short speed) {
        if (mLineTracer.mIsInitialized == false) {
            mLineTracer.mBalancingWalker->init();
            mLineTracer.mIsInitialized = true;
        }

        // 走行体の向きを計算する
        mLineTracer.mPidController->setPidFactor(pidSet[0],pidSet[1],pidSet[2]);
        int direction = calcDirection();

        mLineTracer.mBalancingWalker->setCommand(speed, direction);//速度

        // 倒立走行を行う
        mLineTracer.mBalancingWalker->run();
    };


    // バランス崩す状態の走行 
    runOnOff = []{
        if (mLineTracer.mIsInitialized == false) {
            mLineTracer.mBalancingWalker->init();
            mLineTracer.mIsInitialized = true;
        }

        bool isOnLine = mLineTracer.mLineMonitor ->isOnLine99();//ライン上判定（ジャイロセンサ）
        //走行を行う
        if(isOnLine == true){
            mLineTracer.mLeftWheel.setPWM(15);
            mLineTracer.mRightWheel.setPWM(5);
        }
        else{
            mLineTracer.mLeftWheel.setPWM(5);
            mLineTracer.mRightWheel.setPWM(15);
        }
    };


}

