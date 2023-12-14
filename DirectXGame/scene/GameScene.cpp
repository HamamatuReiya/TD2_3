#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 600;
	viewProjection_.Initialize();

	#pragma region 自キャラ

	//自キャラ関連
	player_ = std::make_unique<Player>();
	modelPlayer_.reset(Model::CreateFromOBJ("Player", true));
	//(自キャラの位置)
	PlayerPosition = {0, 0, 0};
	player_->Initialize(modelPlayer_.get(), PlayerPosition);

	#pragma endregion

	#pragma region 敵

	Enemy* enemy = new Enemy;
	modelEnemy_.reset(Model::CreateFromOBJ("cube", true));
	enemy->Initialize(modelEnemy_.get());
	enemys_.push_back(enemy);

#pragma endregion

	// 天球
	//  3Dモデルの生成
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	// 天球の生成
	skydome_ = std::make_unique<Skydome>();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_.get());

	// カメラの生成
	camera_ = std::make_unique<Camera>();
	// カメラの初期化
	camera_->Initialize();

#ifdef _DEBUG	

	// デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
#endif // _DEBUG

	

}

void GameScene::Update() {

	//プレイヤーの更新
	player_->Update(viewProjection_);

	//敵の更新
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

	//デスフラグの立ったアイテムを削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy)) {

	}

	// カメラの更新
	camera_->Update();
	// 天球の更新
	skydome_->Update();

	// デバッグカメラ
	debugCamera_->Update();

#ifdef _DEBUG
	// Cを押して起動
	if (input_->TriggerKey(DIK_C) && isDebugCameraActive_ == false) {
		isDebugCameraActive_ = true;
	}
	// Cを押して解除
	if (input_->TriggerKey(DIK_V) && isDebugCameraActive_ == true) {
		isDebugCameraActive_ = false;
	}
#endif // _DEBUG

	// カメラの処理
	if (isDebugCameraActive_ == true) {
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.matView = camera_->GetViewProjection().matView;
		viewProjection_.matProjection = camera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}

}

void GameScene::Draw() {

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

	//自キャラの描画
	player_->Draw(viewProjection_);

	//敵の描画
	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	// 天球の描画
	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::sceneReset() {}

void GameScene::CheakAllCollisions() {
	// 対象物A(自機)とB(敵)
	Vector3 posA = {0};
	Vector3 posB = {0};

	// 2間点の距離
	float posAB;

	// 自キャラの半径
	float playerRadius = 10.0f;
	// 敵弾の半径
	float enemyBulletRadius = 10.0f;

#pragma region 自キャラと敵の当たり判定
	// 自キャラのワールド座標を取得
	posA = player_->GetWorldPosition();

	for (Enemy* enemy : enemys_) {
		// 敵のワールド座標を取得
		posB = enemy->GetWorldPosition();

		// AとBの距離を求める
		posAB = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		        (posB.z - posA.z) * (posB.z - posA.z);

		// 球と球との当たり判定
		if (posAB <= (playerRadius + enemyBulletRadius) * (playerRadius + enemyBulletRadius)) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			enemy->OnCollision();
		}
	}
}

