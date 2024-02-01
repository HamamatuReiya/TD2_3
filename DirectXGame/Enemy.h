#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Enemy {
public:
	// デストラクタ

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(
	    Model* model, const Vector3& position, const Vector3& velocity,
	    ViewProjection& viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	void HitJudge(int PlayerAttack);

	void ChangePos(ViewProjection& viewProjection);

public:
	// コールバック関数
	void OnCollision();

	void NotCollision();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	//敵の体力を獲得
	int GetHP() { return enemyHP; }

	//敵の攻撃力を獲得
	int GetPower() { return enemyAttack; }

	bool IsDead() const { return isDead_; }

	// プレイヤーにダメージを与える関数
	float AttackPlayer(float playerHP);

	// ダメージフラグをリセットする関数
	void ResetFlag();

private:

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;

	//敵の体力
	int enemyHP = 2;

	//敵の攻撃力
	int enemyAttack = 6;

	//接触判定
	bool isCollision_ = false;

	// ダメージを与えるフラグ
	bool isDamageFlag_ = true;

	//死亡判定
	bool isDead_ = false;
	// 速度
	Vector3 velocity_;
};
