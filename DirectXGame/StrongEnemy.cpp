#include "StrongEnemy.h"
#include <compute.h>
#include"ImGuiManager.h"

StrongEnemy::StrongEnemy() {}

StrongEnemy::~StrongEnemy() {}

void StrongEnemy::Initialize(
    Model* model, const Vector3& position, const Vector3& velocity,
    ViewProjection& viewProjection) {
	// 引数からデータを受け取る
	model_ = model;

	worldTransform_.scale_ = {8.0f, 8.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = position;

	ChangePos(viewProjection);

	velocity_ = velocity;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void StrongEnemy::Update() {
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	// 敵キャラの横回転
	worldTransform_.rotation_.z += 0.01f;

		////Z軸を固定化
	worldTransform_.translation_.z = 130.0f;

	// 行列の更新
	worldTransform_.UpdateMatrix();

	ImGui::Begin("StrongEnemy");
	ImGui::Text(
	    "model:%f,%f,%f,%d", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z, enemyHP);
	ImGui::End();
}

void StrongEnemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void StrongEnemy::HitJudge(int PlayerAttack) {
	if (isCollision_ == true) {
		isCollision_ = false;
		enemyHP -= PlayerAttack;
	};

	if (enemyHP <= 0) {
		isDead_ = true;
	}
}

void StrongEnemy::ChangePos(ViewProjection& viewProjection) {
	Vector2 EnemyPosition = {worldTransform_.translation_.x, worldTransform_.translation_.y};

	// ビューポート行列
	Matrix4x4 matViewport2 = MakeViewportMatrix(0, 0, 1280, 720, 0, 1);

	// ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV =
	    Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport2);

	// 合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	// スクリーン座標
	Vector3 posNear = Vector3((float)EnemyPosition.x, (float)EnemyPosition.y, 0);
	Vector3 posFar = Vector3((float)EnemyPosition.x, (float)EnemyPosition.y, 1);

	// スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	// マウス例の方向
	Vector3 enemyVector = Subtract(posFar, posNear);
	Vector3 enemyDirection = Add(posNear, enemyVector);

	enemyDirection = Normalize(enemyDirection);

	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 100.0f;

	worldTransform_.translation_ = Add(posNear, Multiply(kDistanceTestObject, enemyDirection));
}

void StrongEnemy::OnCollision() { 
	isCollision_ = false;
}

void StrongEnemy::NotCollision() {
	isCollision_ = true;
}

int StrongEnemy::AttackPlayer(int playerHP) {
	if (isDamageFlag_ == true) {
		playerHP -= enemyAttack;
		isDamageFlag_ = false;
	}
	return playerHP;
}

void StrongEnemy::ResetFlag() { isDamageFlag_ = true; }

Vector3 StrongEnemy::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
