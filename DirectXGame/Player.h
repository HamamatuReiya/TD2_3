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
	
	/// ワールド座標を取得
	Vector3 GetWorldPosition();

	//自分の攻撃力を渡す関数
	int GetAttackPow() { return attackPower; }

	//自分のHPを渡す関数
	float GetHP() { return playerHP_; }

	//敵から減らされたダメージを貰う関数
	void GetDamageAfter(int hp);

public:

	bool IsDead() const { return isDead_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//キーボード入力
	Input* input_ = nullptr;

	//プレイヤーの体力
	const float kPlayerHP_ = 230.0f;
	float playerHP_ = kPlayerHP_;

	// プレイヤーの攻撃力
	int attackPower = 1;

	// 死亡判定
	bool isDead_ = false;

	// 接触判定
	bool isCollision_ = false;
	//ダメージを一瞬だけ受けるためのフラグ
	bool damageFlag_ = true;

};