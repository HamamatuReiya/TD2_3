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

	void Initialize(Model* model,Model* model2, Vector3 position);

	void Update(ViewProjection& viewProjection);

	void Draw(ViewProjection& viewProjection);

	void Move(ViewProjection& viewProjection);

	void Recovery();
	
	/// ワールド座標を取得
	Vector3 GetWorldPosition();

	//自分の攻撃力を渡す関数
	int GetAttackPow() { return attackPower; }

	//自分のHPを渡す関数
	float GetHP() { return playerHP_; }

	//敵から減らされたダメージを貰う関数
	void GetDamageAfter(float hp);

public:

	

	enum class PlayerState {
		isAlive,
		isDead,
	};

	int GetState() { return isDead_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	Model* model2_ = nullptr;
	//キーボード入力
	Input* input_ = nullptr;

	//プレイヤーの体力
	const float kPlayerHP_ = 300.0f;
	float playerHP_ = kPlayerHP_;
	float recoveryPower = 15.0f;

	// プレイヤーの攻撃力
	int attackPower = 1;

	// 死亡判定
	bool isDead_ = false;

	// 接触判定
	bool isCollision_ = false;
	//ダメージを一瞬だけ受けるためのフラグ
	bool damageFlag_ = true;

	//////////プレイヤーが倒れたときに連打で回復するのに使う物たち↓
	
	PlayerState playerState = PlayerState::isAlive;

};