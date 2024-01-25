#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include "Time.h"
#include <compute.h>

class Camera {
public:
	/// <summary
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary
	/// 更新
	/// </summary>
	void Update();

	//カメラ揺れ
	void CameraShake();
	
	//カメラ揺れを発生させるフラグ
	void ShakeStart(int HP);

public:
	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	const WorldTransform& GetWorldTransform() { return worldTransform_; }


private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	//シェイクのフラグ
	bool shakeFlag = false;
	//シェイクのタイマー
	int shakeTimer = 0;
	// 揺れる値を入れる変数
	int shakeTransX = 0;
	int shakeTransY = 0;

};
