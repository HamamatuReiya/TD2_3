#include "Enemy.h"
#include <compute.h>
#include"ImGuiManager.h"

void Enemy::Initialize(
    Model* model, const Vector3& position, const Vector3& velocity,
    ViewProjection& viewProjection) {
	// 引数からデータを受け取る
	model_ = model;
	
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = position;

	velocity_ = velocity;

	ChangePos(viewProjection);


	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Enemy::Update() {

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	velocity_.y -= 0.01f;

	// 行列の更新
	worldTransform_.UpdateMatrix();

		ImGui::Begin("ENEMY");
	    ImGui::Text(
	    "model:%f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	    ImGui::End();
}

void Enemy::Draw(ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection); }

void Enemy::HitJudge(int PlayerAttack) { 
	if (isCollision_ == true) {
		    isCollision_ = false;
		    enemyHP -= PlayerAttack;
	};
	
	if (enemyHP <= 0) {
	        isDead_ = true;    
	}
}

void Enemy::ChangePos(ViewProjection& viewProjection) {
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

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() { isCollision_ = false; }

void Enemy::NotCollision() { isCollision_ = true; }
