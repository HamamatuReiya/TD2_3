#pragma once
#include "Model.h"
#include "WorldTransform.h"

class StrongEnemy {
public:
	/// <summary>
	/// コンストクラタ
	/// </summary>
	StrongEnemy();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StrongEnemy();

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

	void ChangePos(ViewProjection& viewProjection);

public:

	void HitJudge(int PlayerAttack);

	void NotCollision();

	// コールバック関数
	void OnCollision();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	bool IsDead() const { return isDead_; }

	// 敵の体力を獲得
	int GetHP() { return enemyHP; }

	// 敵の攻撃力を獲得
	int GetPower() { return enemyAttack; }

	//プレイヤーにダメージを与える関数
	float AttackPlayer(float playerHP);

	//ダメージフラグをリセットする関数
	void ResetFlag();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;

	// 敵の体力
	int enemyHP = 8;

	// 敵の攻撃力
	int enemyAttack = 7;

	// 接触判定
	bool isCollision_ = false;

	//ダメージを与えるフラグ
	bool isDamageFlag_ = true;

	bool isDead_ = false;
	// 速度
	Vector3 velocity_;
};
