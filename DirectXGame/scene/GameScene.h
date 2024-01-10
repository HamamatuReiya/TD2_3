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
#include "Scene.h"

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

public:

	// シーンのリセット
	void sceneReset();

	//衝突判定
	void CheakAllCollisions();

private:
	/// <summary>
	/// 敵の発生
	/// </summary>
	void EnemySpawn(Vector3 position, Vector3 velocity);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

public:
	bool isSceneEnd = false;

	bool IsSceneEnd() { return isSceneEnd; }
	SceneType NextScene() { return SceneType::kTitle; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	//自キャラの3Dモデル
	std::unique_ptr<Model> modelPlayer_;

	//自キャラのモデル
	std::unique_ptr<Player> player_;

	Vector3 PlayerPosition;

	// 敵リスト
	std::list<Enemy*> enemys_;
	// 敵の3Dモデル
	std::unique_ptr<Model> modelEnemy_;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;
	// 敵の待機中のフラグ
	bool enemyPopWaitFlag = true;
	// 待機タイマー
	int32_t enemyPopWaitTimer = 0;
	
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
