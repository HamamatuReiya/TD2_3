#include "Audio.h"
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "WinApp.h"

#include "TitleScene.h"
#include "GameOverScene.h"
#include "GameClearScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;
	GameScene* gameScene = nullptr;
	TitleScene* titleScene = nullptr;
	GameClearScene* gameClearScene = nullptr;
	GameOverScene* gameOverScene = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow();

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize();

	// タイトルシーンの初期化
	titleScene = new TitleScene();
	titleScene->Initialize();

	// ゲームクリアシーンの初期化
	gameClearScene = new GameClearScene();
	gameClearScene->Initialize();

	// ゲームオーバーシーンの初期化
	gameOverScene = new GameOverScene();
	gameOverScene->Initialize();

	SceneType sceneNo = SceneType::kTitle;

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();

		switch (sceneNo) {
		case SceneType::kTitle:
			titleScene->Update();

			if (titleScene->IsSceneEnd()) {
				// 次のシーンの値を代入してシーン切り替え
				sceneNo = titleScene->NextScene();

				// タイトルシーンの初期化、フラグリセット等
				titleScene->sceneReset();
			}

			break;

		case SceneType::kGamePlay:
			// ゲームシーンの毎フレーム処理
			gameScene->Update();

			if (gameScene->IsWave1End()) {
				if (gameScene->IsUpgradeEnd()) {
					gameScene->NextWave2();
					gameScene->Wave2Initialize();
					gameScene->WaveReset();
				}
			}
			if (gameScene->IsWave2End()) {
				if (gameScene->IsUpgradeEnd()) {
					gameScene->NextWave3();
					gameScene->Wave3Initialize();
					gameScene->WaveReset();
				}
			}

			if (gameScene->IsSceneEnd()) {
				// 次のシーンの値を代入してシーン切り替え
				sceneNo = gameScene->NextScene();

				// ゲームシーンの初期化、フラグリセット等
				gameScene->sceneReset();
			}
			if (gameScene->IsGameOver()) {
				sceneNo = gameScene->GameOverScene();
			}

			break;
		case SceneType::kGameClear:
			gameClearScene->Update();

			if (gameClearScene->IsSceneEnd()) {
				// 次のシーンの値を代入してシーン切り替え
				sceneNo = gameClearScene->NextScene();

				// タイトルシーンの初期化、フラグリセット等
				gameClearScene->sceneReset();
			}
		break;

		case SceneType::kGameOver:
		gameOverScene->Update();

		if (gameOverScene->IsSceneEnd()) {
				// 次のシーンの値を代入してシーン切り替え
				sceneNo = gameOverScene->NextScene();

				// タイトルシーンの初期化、フラグリセット等
				gameOverScene->sceneReset();
			}
		break;
		}

		// 軸表示の更新
		axisIndicator->Update();
		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();

		switch (sceneNo) {
		case SceneType::kTitle:
			titleScene->Draw();
			break;
		case SceneType::kGamePlay:
			gameScene->Draw();
			break;
		case SceneType::kGameClear:
			gameClearScene->Draw();
		    break;
		case SceneType::kGameOver:
			gameOverScene->Draw();
			break;
		}
		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	SafeDelete(gameScene);
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}