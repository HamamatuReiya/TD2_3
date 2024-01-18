#include "StrongEnemy.h"
#include <compute.h>
#include"ImGuiManager.h"

StrongEnemy::StrongEnemy() {}

StrongEnemy::~StrongEnemy() {}

void StrongEnemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// 引数からデータを受け取る
	model_ = model;

	worldTransform_.scale_ = {2.5f, 2.5f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = position;

	velocity_ = velocity;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void StrongEnemy::Update() {
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	// 敵キャラの横回転
	worldTransform_.rotation_.z += 0.01f;

	// 行列の更新
	worldTransform_.UpdateMatrix();

	ImGui::Begin("StrongEnemy");
	ImGui::Text(
	    "model:%f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();
}

void StrongEnemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void StrongEnemy::OnCollision() { isDead_ = true; }

Vector3 StrongEnemy::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
