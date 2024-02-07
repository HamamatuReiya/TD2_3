#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Borderline {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

    /// <summary>
    /// /耐久値関連
    /// </summary>
public:
	//耐久値を減らす
	void ReduceEND();

	//耐久値を最大耐久値まで戻す
	void ResetEND();

	//ゲームオーバーのフラグを返す
	int ReturnFlag();

public:
	int GetLineEND() { return lineEND; }

	void ResetFlag();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;

	//防衛線の最大耐久値
	int maxLineEND = 5;
	//防衛線の耐久値
	int lineEND;
	//ゲームオーバーのフラグ
	int breakLineFlag=false;
	

};
