#include "Camera.h"
#include "compute.h"
#include"ImGuiManager.h"

void Camera::Initialize() {
	// x,y,z方向の回転を設定
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	// x,y,zの方向のを設定
	worldTransform_.translation_ = {0.0f, 0.0f, -50.0f};

	/// 乱数の初期化(シード値の設定)
	srand((unsigned int)time(nullptr));

	

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void Camera::Update() {

	//カメラのViewProjectionをもう一つ用意して、自機とカメラのシェイクに使っている変数を別々にする

	CameraShake();

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    {1.0f, 1.0f, 1.0f}, worldTransform_.rotation_, worldTransform_.translation_);
	
	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	
	
	//viewProjection_.UpdateMatrix();

}

void Camera::CameraShake() {
	if (shakeFlag == true){

		//タイマーをプラス
		shakeTimer++;
		// 揺れる値を入れる変数
		shakeTransX = 0;
		shakeTransY = 0;

		// 揺れる分のVector3
		Vector3 shakeTrans = {(float)shakeTransX, (float)shakeTransY, 0};

		shakeTransX = rand() % 3 - 1;
		shakeTransY = rand() % 3 - 1;

		// worldTransform_.translation_ = Add(worldTransform_.translation_, shakeTrans);
		worldTransform_.translation_.x += shakeTransX;
		worldTransform_.translation_.y += shakeTransY;
	} else {
		worldTransform_.translation_ = {0.0f, 0.0f, -50.0f};
		// 揺れる値をリセット
		shakeTransX = 0;
		shakeTransY = 0;
	}

	if (shakeTimer >= 10) {
		shakeTimer = 0;
		shakeFlag = false;
	}

	worldTransform_.UpdateMatrix();

}

void Camera::ShakeStart(int HP) { 
	if (HP <= 0) {
		shakeFlag = true;
	}
}
