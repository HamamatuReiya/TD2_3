#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "compute.h"

class Player {
public:

	void Initialize(Model* model, Vector3 position);

	void Update(ViewProjection& viewProjection);

	void Draw(ViewProjection& viewProjection);
	
	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// コールバック関数
	void OnCollision();

	void NotCollision();

	//自分の攻撃力を渡す関数
	int GetAttackPow() { return attackPower; }

	int GetHP() { return HP_; }

public:
	void HitJudge();

	bool IsDead() const { return isDead_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//キーボード入力
	Input* input_ = nullptr;

	//プレイヤーの体力
	const int kPlayerHP_ = 30;
	int playerHP_ = kPlayerHP_;

	// プレイヤーの攻撃力
	int attackPower = 1;

	// 死亡判定
	bool isDead_ = false;

	// 接触判定
	bool isCollision_ = false;

	// HP
	int HP_;
};