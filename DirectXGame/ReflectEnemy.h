#pragma once
#include "Model.h"
#include "WorldTransform.h"

class ReflectEnemy {
public:
	/// <summary>
	/// コンストクラタ
	/// </summary>
	ReflectEnemy();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ReflectEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(
	    Model* model, const Vector3& position, const Vector3& velocity,
	    ViewProjection& viewProjection);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	void HitJudge(int PlayerAttack);

	void ChangePos(ViewProjection& viewProjection);

public:
	void NotCollision();

	// コールバック関数
	void OnCollision();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	bool IsDead() const { return isDead_; }

private:
	//反射の動作の関数
	void reflectUpdate();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;

		// 敵の体力
	int enemyHP = 2;

	// 接触判定
	bool isCollision_ = false;

	// 死亡判定
	bool isDead_ = false;
	// 速度
	Vector3 velocity_;
};
