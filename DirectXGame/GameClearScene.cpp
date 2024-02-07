#include "GameClearScene.h"

GameClearScene::GameClearScene() {}

GameClearScene::~GameClearScene() {}

void GameClearScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// タイトルのテクスチャ読み込み
	gameClearTexture_ = TextureManager::Load("clear.png");
	// タイトルの生成
	gameClearSprite_ = Sprite::Create(gameClearTexture_, {0, 0});
}

void GameClearScene::Update() {
	if (input_->TriggerKey(DIK_SPACE)) {
		isSceneEnd = true;
	}
}

void GameClearScene::Draw() {
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

	gameClearSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameClearScene::sceneReset() {
	// シーン移行のリセット
	isSceneEnd = false;
}
