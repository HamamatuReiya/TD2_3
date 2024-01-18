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

	//自分の攻撃力を渡す関数
	int GetAttackPow() { return attackPower; }

	private:

	//プレイヤーの攻撃力
	int attackPower = 1;

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//キーボード入力
	Input* input_ = nullptr;

};