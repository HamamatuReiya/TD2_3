#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

#include <memory>

#include"Player.h"
#include"Enemy.h"
#include "Skydome.h"
#include "Camera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//自キャラの3Dモデル
	std::unique_ptr<Model> modelPlayer_;

	//自キャラのモデル
	std::unique_ptr<Player> player_;

	Vector3 PlayerPosition;

	// 敵の3Dモデル
	std::unique_ptr<Model> modelEnemy_;

	// 敵のモデル
	std::unique_ptr<Enemy> enemy_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_; 

	// 固定カメラ
	std::unique_ptr<Camera> camera_;
	
	// 天球
	std::unique_ptr<Skydome> skydome_;
	//   3Dモデル
	std::unique_ptr<Model> modelSkydome_;
};
