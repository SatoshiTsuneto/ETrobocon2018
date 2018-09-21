#include "BehaviorHolder.h"
#include "LineTracer.h"
#include "LineMonitor.h"
#include "BalancingWalker.h"
#include "PidController.h"
#include "ev3api.h"
#include "SonarSensor.h"

#include <map>
#include <functional>

//コンストラクタ
BehaviorHolder::BehaviorHolder(LineTracer *lineTracer) : mLineTracer(lineTracer) {
    init();
}

BehaviorHolder::~BehaviorHolder() = default;

std::function<void(void)> BehaviorHolder::findBehaviorById(BehaviorId id) {
    std::function<void(void)> behavior = behavior_map[id];
    return behavior;
}

/**
 * 走行体の向きを計算する
 * @param isOnLine true:ライン上/false:ライン外
 * @retval 30  ライン上にある場合(右旋回指示)
 * @retval -30 ライン外にある場合(左旋回指示)
 */
int BehaviorHolder::calcDirection() {
    return static_cast<int>(mLineTracer->mPidController->calControlledVariable(
            mLineTracer->mLineMonitor->getDeviation()));
}

// ラムダ式の実装
void BehaviorHolder::init() {

    // 走行体を初期状態にする
    behavior_map[ID_INITIALIZE] = [this] {
        mLineTracer->mTailWheel.reset();
        mLineTracer->mTailWheel.setCount(0);
        mLineTracer->motor_control(mLineTracer->mTailWheel, 85, 50); //テイル降ろす
        mLineTracer->mCalibration->init();
    };

    // キャリブレーション
    behavior_map[ID_CALIBRATION] = [this] {

        // TODO : 汚いまで
        bool result = false;

        result = mLineTracer->mCalibration->calibrateGyro(true);
        result = mLineTracer->mCalibration->calibrateBlack(true);
        result = mLineTracer->mCalibration->calibrateWhite(mLineTracer->mStarter->isPushed());
        result = mLineTracer->mCalibration->calibrationColor(mLineTracer->mStarter->isPushed());

        if (result) {
            while (mLineTracer->mStarter->isPushed());
        }
    };

    // // スタートダッシュ
    // startDashByBluetooth = [](int* mBt_data){
    //     if (mLineTracer.mStarter->isPushed() || *mBt_data == '1') {//ボタン押すＯＲブルーツースから１送るとスタート
    //         mLineTracer.mCalibration->calibrateLineThreshold();
    //         mLineTracer.motor_control(mTailWheel,100,50);
    //         // tailモータの押し出す力によって、左右ホイールが3°動いたら次のStateに移る
    //         mLineTracer.mTailWheel.setPWM(-50);
    //     }
    // };


    // 直線走行
    behavior_map[ID_NORMAL_RUN] = [this] {

        float *pid = mLineTracer->get_pid();
        if (!mLineTracer->mIsInitialized) {
            mLineTracer->mBalancingWalker->init();
            mLineTracer->mIsInitialized = true;
        }

        // 走行体の向きを計算する
        mLineTracer->mPidController->setPidFactor(pid[0], pid[1], pid[2]);
        int direction = calcDirection();

        mLineTracer->mBalancingWalker->setCommand(mLineTracer->get_spped(), direction);//速度

        // 倒立走行を行う
        mLineTracer->mBalancingWalker->run();
    };


    // バランス崩す状態の走行
    behavior_map[ID_RUNONOFF] = [this] {
        if (!mLineTracer->mIsInitialized) {
            mLineTracer->mBalancingWalker->init();
            mLineTracer->mIsInitialized = true;
        }

        bool isOnLine = mLineTracer->mLineMonitor->isOnLine99();//ライン上判定（ジャイロセンサ）
        //走行を行う
        if (isOnLine) {
            mLineTracer->mLeftWheel.setPWM(15);
            mLineTracer->mRightWheel.setPWM(5);
        } else {
            mLineTracer->mLeftWheel.setPWM(5);
            mLineTracer->mRightWheel.setPWM(15);
        }
    };
}






