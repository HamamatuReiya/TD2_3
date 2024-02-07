﻿#include "GameOverScene.h"

GameOverScene::GameOverScene() {}

GameOverScene::~GameOverScene() {}

void GameOverScene::Initialize() { 
	// タイトルのテクスチャ読み込み
	gameOverTexture_ = TextureManager::Load("gameover.png");
	// タイトルの生成
	gameOverSprite_ = Sprite::Create(gameOverTexture_, {0, 0});
	}

void GameOverScene::Update() {
	/*if (input_->TriggerKey(DIK_SPACE)) {
		isSceneEnd = true;
	}*/
    }

void GameOverScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	gameOverSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
 }

void GameOverScene::sceneReset() {
	// シーン移行のリセット
	isSceneEnd = false;
}
