#include "ReflectEnemy.h"
#include <compute.h>
#include"ImGuiManager.h"

ReflectEnemy::ReflectEnemy() {}

ReflectEnemy::~ReflectEnemy() {}

void ReflectEnemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// 引数からデータを受け取る
	model_ = model;

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = position;

	velocity_ = velocity;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void ReflectEnemy::Update() {
	
	reflectUpdate();
	// 行列の更新
	worldTransform_.UpdateMatrix();

	ImGui::Begin("ReflectENEMY");
	ImGui::Text(
	    "model:%f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();
}

void ReflectEnemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void ReflectEnemy::reflectUpdate() {
	// x 60 //y 40
	// 移動限界座標
	const float kMoveLimitX = 35;

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	if (worldTransform_.translation_.x <= -kMoveLimitX) {
		velocity_.x *= -1;
	}
	if (worldTransform_.translation_.x >= kMoveLimitX) {
		velocity_.x *= -1;
	}

}

void ReflectEnemy::OnCollision() { isDead_ = true; }

Vector3 ReflectEnemy::GetWorldPosition() { 
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}