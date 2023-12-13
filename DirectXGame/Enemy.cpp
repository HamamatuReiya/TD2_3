#include "Enemy.h"

void Enemy::Initialize(Model* model) {
	// 引数からデータを受け取る
	model_ = model;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 20.0f, 50.0f};
}

void Enemy::Update() {
	worldTransform_.translation_.y -= 0.05f;
	// 行列の更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection);
}
