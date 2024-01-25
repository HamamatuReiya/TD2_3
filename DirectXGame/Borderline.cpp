#include "Borderline.h"

void Borderline::Initialize(Model* model) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {4.0f, 10.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, -70.0f, 130.0f};
}

void Borderline::Update() {
	// 行列の更新
	worldTransform_.UpdateMatrix();
}

void Borderline::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection); 
}
