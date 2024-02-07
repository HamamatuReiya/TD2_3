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
#include "Borderline.h"
#include "PlayerUpgrade.h"

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

	//敵がラインに触れたら耐久値を減らす
	void DamageLine();

private:
	enum class gameState { 
		Wave, 
		Upgrade,
	};

	gameState stateNo = gameState::Wave;

	void Wave1Initialize();
	void Wave2Initialize();

	void WaveReset();

	enum class Wave {
		Wave1,
		Wave2,
		Wave3
	};
	
	bool isWave1End = false;
	bool isWave2End = false;

	bool IsWave1End() { return isWave1End; }
	bool IsWave2End() { return isWave2End; }

	Wave NextWave2() { return Wave::Wave2; }
	Wave NextWave3() { return Wave::Wave3; }

	void Upgrade();
	
	void IsUpgradeEndReset();

	bool isUpgrade1End = false;
	bool IsUpgrade1End() { return isUpgrade1End; }

	bool isUpgrade2End = false;
	bool IsUpgrade2End() { return isUpgrade2End; }

public:

	bool isSceneEnd = false;

	bool IsSceneEnd() { return isSceneEnd; }
	SceneType NextScene() { return SceneType::kTitle; }

	//ゲームオーバー関係（シーン）
	bool isGameOver = false;

	bool IsGameOver() { return isGameOver; }
	SceneType GameOverScene() { return SceneType::kGameOver; }

private:
	Wave waveNo1 = Wave::Wave1;
	Wave waveNo2 = Wave::Wave2;

private:
	void TextureInitialize();

	void BreakLineHPUpdate(int32_t num);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	//自キャラの3Dモデル
	std::unique_ptr<Model> modelPlayer_;

	std::unique_ptr<Model> modelDeadPlayer_;

	//自キャラのモデル
	std::unique_ptr<Player> player_;

	std::unique_ptr<Player> deadPlayer_;

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
	// 敵の死んだ数
	int enemyDeath_ = 0;

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

	//ボーダーライン
	std::unique_ptr<Borderline> borderline_;
	// 3Dモデル
	std::unique_ptr<Model> modelBorderline_;
	// ボーダーラインの上限
	int border_ = 5;

	//アップグレード
	std::unique_ptr<PlayerUpgrade> playerUpgrade_;


	// プレイヤーのHP
	uint_fast32_t playerHPTexture_ = 0;
	// スプライト
	Sprite* playerHPSprite_ = nullptr;
	//表示範囲
	Vector2 HP_;

	// プレイヤーのHPバー
	uint_fast32_t playerHPberTexture_ = 0;
	// スプライト
	Sprite* playerHPberSprite_ = nullptr;
	//表示範囲
	Vector2 HPber_;

	////強化画面用のスプライト
	//攻撃
	uint32_t attackTexture_ = 0;
	Sprite* attackSprite_ = nullptr;
	//体力
	uint32_t lifeTexture_ = 0;
	Sprite* lifeSprite_ = nullptr;
	//回復力
	uint32_t recoveryTexture_ = 0;
	Sprite* recoverySprite_ = nullptr;

	//ボーダーラインの体力表示
	int32_t num1;
	Sprite* textureNumber_[10];
	Vector2 texturePos = {640, 650};
};
