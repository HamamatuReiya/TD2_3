﻿#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Enemy {
public:
	// デストラクタ

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model,const Vector3& position,const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

public:
	// コールバック関数
	void OnCollision();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	bool IsDead() const { return isDead_; }

private:

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;

	bool isDead_ = false;
	// 速度
	Vector3 velocity_;
};
