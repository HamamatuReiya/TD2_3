﻿#include "CurveEnemy.h"
#include <compute.h>
#include"ImGuiManager.h"
#define _USE_MATH_DEFINES
#include <math.h>

CurveEnemy::CurveEnemy() {}

CurveEnemy::~CurveEnemy() {}

void CurveEnemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// 引数からデータを受け取る
	model_ = model;

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = position;

	velocity_ = velocity;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void CurveEnemy::Update() {
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	// 行列の更新
	worldTransform_.UpdateMatrix();

	ImGui::Begin("StrongEnemy");
	ImGui::Text(
	    "model:%f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();
}

void CurveEnemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void CurveEnemy::OnCollision() { isDead_ = true; }

Vector3 CurveEnemy::GetWorldPosition() { 
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void CurveEnemy::InitializeCurveGimmick() { curveParameter_ = 0.0f; }

void CurveEnemy::UpdateCurveGimmick() {
	// 浮遊移動のサイクル<frame>
	const uint16_t kPeriod = uint16_t(kCurveFPS);
	// 1フレームでのパラメータ加算値
	const float kStep = 2.0f * float(M_PI) / float(kPeriod);

	// パラメータを1ステップ分加算
	curveParameter_ += kStep;
	// 2πを超えたら0に戻す
	curveParameter_ = std::fmod(curveParameter_, 2.0f * float(M_PI));

	// 浮遊の振幅<m>
	const float floatingSwing = 0.1f;
	// 浮遊を座標に反映
	worldTransform_.translation_.x = std::sin(curveParameter_) * floatingSwing;
}

