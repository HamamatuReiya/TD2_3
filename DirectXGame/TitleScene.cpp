#include "TitleScene.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {}

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// タイトルのテクスチャ読み込み
	titleTexture_ = TextureManager::Load("title3.png");
	// タイトルの生成
	titleSprite_ = Sprite::Create(titleTexture_, {0, 0});

	// タイトルのテクスチャ読み込み
	tutorialTexture_ = TextureManager::Load("tutorial.png");
	// タイトルの生成
	tutorialSprite_ = Sprite::Create(tutorialTexture_, {0, 0});

}

void TitleScene::Update() {
	if (input_->IsTriggerMouse(0) && isTutorial == true) {
		isSceneEnd = true;
	}

	if (input_->IsTriggerMouse(0) && isTutorial == false) {
		isTutorial = true;
	}
}

void TitleScene::Draw() {
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

	if (isTutorial == false) {
		titleSprite_->Draw();
	} else {
		tutorialSprite_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void TitleScene::sceneReset() { 
	//シーン移行のリセット
	isSceneEnd = false;
	// 遊び方のフラグのリセット
	isTutorial = false;
}