#pragma once
#include "Model.h"
#include "WorldTransform.h"

class CurveEnemy {
public:
	/// <summary>
	/// コンストクラタ
	/// </summary>
	CurveEnemy();
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~CurveEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(
	    Model* model, const Vector3& position, const Vector3& velocity, bool directionFlag,
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
	// コールバック関数
	void OnCollision();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	bool IsDead() const { return isDead_; }

private:
	//カーブの初期化
	void InitializeCurveGimmick();
	//カーブの更新
	void UpdateCurveGimmick();
	
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;

	bool isDead_ = false;
	// 速度
	Vector3 velocity_;

	// カーブギミックの媒介変数
	float curveParameter_ = 0.0f;

	// フレーム数
	const float kCurveFPS = 10.0f * 60.0f;

	//左右のフラグ
	bool directionFlag_ = false;

	float direction_ = 0;

	// 敵の体力
	int enemyHP = 2;

	// 接触判定
	bool isCollision_ = false;
};
