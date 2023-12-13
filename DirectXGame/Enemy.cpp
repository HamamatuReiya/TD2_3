#include "Enemy.h"

void Enemy::Initialize(Model* model) {
	// ��������f�[�^���󂯎��
	model_ = model;
	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 50.0f};
}

void Enemy::Update() {
	// �s��̍X�V
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection);
}
