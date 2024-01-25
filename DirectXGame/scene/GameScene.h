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
#include "StrongEnemy.h"
#include "ReflectEnemy.h"
#include "CurveEnemy.h"

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

	//敵のHPを受け取ってカメラを揺らす関数
	void CheakHPCameraShake();

private:
	/// <summary>
	/// 敵の発生
	/// </summary>
	void EnemySpawn(Vector3 position, Vector3 velocity);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData(const std::string& fileName);

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	/// <summary>
	/// 強めの敵の発生
	/// </summary>
	void StrongEnemySpawn(Vector3 position, Vector3 velocity);

	/// <summary>
	/// 強めの敵発生データの読み込み
	/// </summary>
	void LoadStrongEnemyPopData(const std::string& fileName);

	/// <summary>
	/// 強めの敵発生コマンドの更新
	/// </summary>
	void UpdateStrongEnemyPopCommands();

	/// <summary>
	/// 反射する敵の発生
	/// </summary>
	void ReflectEnemySpawn(Vector3 position, Vector3 velocity);

	/// <summary>
	/// 反射する敵発生データの読み込み
	/// </summary>
	void LoadReflectEnemyPopData(const std::string& fileName);

	/// <summary>
	/// 反射する敵発生コマンドの更新
	/// </summary>
	void UpdateReflectEnemyPopCommands();

   /// <summary>
	/// 曲がる敵の発生
	/// </summary>
	void CurveEnemySpawn(Vector3 position, Vector3 velocity, bool directionFlag);

	/// <summary>
	/// 曲がる敵発生データの読み込み
	/// </summary>
	void LoadCurveEnemyPopData(const std::string& fileName);

	/// <summary>
	/// 曲がる敵発生コマンドの更新
	/// </summary>
	void UpdateCurveEnemyPopCommands();

	/// <summary>
	/// ボーダーライン更新
	/// </summary>
	void UpdateBorderLine();

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

	// 強めの敵リスト
	std::list<StrongEnemy*> strongEnemys_;
	// 強めの敵の3Dモデル
	std::unique_ptr<Model> modelStrongEnemy_;

	// 強めの敵発生コマンド
	std::stringstream strongEnemyPopCommands;
	// 強めの敵の待機中のフラグ
	bool strongEnemyPopWaitFlag = true;
	// 強めの敵の待機タイマー
	int32_t strongEnemyPopWaitTimer = 0;
	
	// 反射する敵リスト
	std::list<ReflectEnemy*> reflectEnemys_;
	// 反射する敵の3Dモデル
	std::unique_ptr<Model> modelReflectEnemy_;

	// 反射する敵発生コマンド
	std::stringstream reflectEnemyPopCommands;
	// 反射する敵の待機中のフラグ
	bool reflectEnemyPopWaitFlag = true;
	// 反射する敵の待機タイマー
	int32_t reflectEnemyPopWaitTimer = 0;

	// 曲がる敵リスト
	std::list<CurveEnemy*> curveEnemys_;
	// 曲がるの3Dモデル
	std::unique_ptr<Model> modelCurveEnemy_;

	// 曲がる敵発生コマンド
	std::stringstream curveEnemyPopCommands;
	// 曲がるの待機中のフラグ
	bool curveEnemyPopWaitFlag = true;
	// 曲がる敵の待機タイマー
	int32_t curveEnemyPopWaitTimer = 0;

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

	// ボーダーライン
	int border_ = 5;
};
