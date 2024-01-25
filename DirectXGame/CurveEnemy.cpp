#include "CurveEnemy.h"
#include <compute.h>
#include"ImGuiManager.h"
#define _USE_MATH_DEFINES
#include <math.h>

CurveEnemy::CurveEnemy() {}

CurveEnemy::~CurveEnemy() {}

void CurveEnemy::Initialize(
    Model* model, const Vector3& position, const Vector3& velocity, bool directionFlag,
	    ViewProjection& viewProjection) {
	// 引数からデータを受け取る
	model_ = model;

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = position;

	velocity_ = velocity;

	directionFlag_ = directionFlag;

	ChangePos(viewProjection);

	InitializeCurveGimmick();

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void CurveEnemy::Update() {
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	UpdateCurveGimmick();

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

void CurveEnemy::InitializeCurveGimmick() { 
	curveParameter_ = 0.0f;

	if (directionFlag_ == false) {
		direction_ = -1;
	}
	if (directionFlag_ == true) {
		direction_ = 1;
	}
}

void CurveEnemy::UpdateCurveGimmick() {
	// 左右移動のサイクル<frame>
	const uint16_t kPeriod = uint16_t(kCurveFPS);
	// 1フレームでのパラメータ加算値
	const float kStep = 5.0f * float(M_PI) / float(kPeriod);

	// パラメータを1ステップ分加算
	curveParameter_ += kStep;
	// 2πを超えたら0に戻す
	curveParameter_ = std::fmod(curveParameter_, 2.0f * float(M_PI));

	// 左右の振幅<m>
	float curveSwing = 20.0f * direction_;
	// 左右を座標に反映
	worldTransform_.translation_.x = std::sin(curveParameter_) * curveSwing;
}

void CurveEnemy::HitJudge(int PlayerAttack) {
	if (isCollision_ == true) {
		isCollision_ = false;
		enemyHP -= PlayerAttack;
	};

	if (enemyHP <= 0) {
		isDead_ = true;
	}
}

void CurveEnemy::ChangePos(ViewProjection& viewProjection) {
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
