#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Scene.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class GameClearScene {
public:
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameClearScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameClearScene();

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

	void sceneReset();

	bool isSceneEnd = false;

	bool IsSceneEnd() { return isSceneEnd; }
	SceneType NextScene() { return SceneType::kTitle; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// タイトル画面
	uint32_t gameClearTexture_ = 0;
	// スプライト
	Sprite* gameClearSprite_ = nullptr;

};
