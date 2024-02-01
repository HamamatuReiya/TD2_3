#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include "AxisIndicator.h"
#include <fstream>
#include "Vector3.h"

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
	// 敵の生成
	modelEnemy_.reset(Model::CreateFromOBJ("Enemy", true));
	// 強め(硬め)の生成
	modelStrongEnemy_.reset(Model::CreateFromOBJ("strongEnemy", true));
	// 反射する敵の生成
	modelReflectEnemy_.reset(Model::CreateFromOBJ("reflectEnemy", true));
	// 曲がる敵の生成
	modelCurveEnemy_.reset(Model::CreateFromOBJ("curveEnemy", true));
	Wave1Initialize();
#pragma endregion

	// 天球
	//  3Dモデルの生成
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	// 天球の生成
	skydome_ = std::make_unique<Skydome>();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_.get());

	// ボーダーラインの生成
	borderline_ = std::make_unique<Borderline>();
	// 3Dモデルの生成
	modelBorderline_.reset(Model::CreateFromOBJ("borderline", true));
	// ボーダーラインの初期化
	borderline_->Initialize(modelBorderline_.get());

	// プレイヤーのHPのテクスチャ読み込み
	playerHPTexture_ = TextureManager::Load("hp.png");
	// プレイヤーのHPの生成
	playerHPSprite_ = Sprite::Create(playerHPTexture_, {0, 0});

	// カメラの生成
	camera_ = std::make_unique<Camera>();
	// カメラの初期化
	camera_->Initialize();

	// デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

#ifdef _DEBUG	

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
#endif // _DEBUG
}

void GameScene::Update() {

	//プレイヤーの更新
	player_->Update(viewProjection_);

	// 敵キャラの更新
	UpdateEnemyPopCommands();
	// 強めの敵の更新
	UpdateStrongEnemyPopCommands();
	// 反射する敵の更新
	UpdateReflectEnemyPopCommands();
	// 曲がる敵の更新
	UpdateCurveEnemyPopCommands();

	//プレイヤーが死亡してるときは敵との接触判定を行わないようにする
	if (player_->GetHP() > 0.0f&&player_->GetState()==0) {
		// 当たり判定
		CheakAllCollisions();
	}

	// プレイヤーHP
	HPber_ = playerHPSprite_->GetSize();
	HPber_.x = player_->GetHP();

	playerHPSprite_->SetSize(HPber_);

	

	// 敵の更新
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
	// 強い敵
	for (StrongEnemy* strongEnemy : strongEnemys_) {
		strongEnemy->Update();
	}
	// 反射する敵
	for (ReflectEnemy* reflectEnemy : reflectEnemys_) {
		reflectEnemy->Update();
	}
	// 曲がる敵
	for (CurveEnemy* curveEnemy : curveEnemys_) {
		curveEnemy->Update();
	}
	
	
	//敵の消滅
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		if (enemy->GetWorldPosition().y <= -90.0f) {
			delete enemy;
			return true;
		}
		return false;
	});

	// 強い敵の消滅
	strongEnemys_.remove_if([](StrongEnemy* strongEnemy) {
		if (strongEnemy->IsDead()) {
			delete strongEnemy;
			return true;
		}
		if (strongEnemy->GetWorldPosition().y <= -80.0f) {
			delete strongEnemy;
			return true;
		}
		return false;
	});

	// 反射する敵の消滅
	reflectEnemys_.remove_if([](ReflectEnemy* reflectEnemy) {
		if (reflectEnemy->IsDead()) {
			delete reflectEnemy;
			return true;
		}
		if (reflectEnemy->GetWorldPosition().y <= -80.0f) {
			delete reflectEnemy;
			return true;
		}
		return false;
	});

	//カーブする敵の消滅
	curveEnemys_.remove_if([](CurveEnemy* curveEnemy) {
		if (curveEnemy->IsDead()) {
			delete curveEnemy;
			return true;
		}
		if (curveEnemy->GetWorldPosition().y <= -80.0f) {
			delete curveEnemy;
			return true;
		}
		return false;
	});


	////デスフラグの立ったアイテムを削除
	//enemys_.remove_if([](std::unique_ptr<Enemy>& enemy)) {
	//
	//}

	// カメラの更新
	camera_->Update();

	// 天球の更新
	skydome_->Update();

	// ボーダーラインの更新
	borderline_->Update();


#ifdef _DEBUG

	// デバッグカメラ
	debugCamera_->Update();

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

	//ウェーブ切り替えデバッグ用
	/*if (input_->TriggerKey(DIK_SPACE)) {
		isWaveEnd = true;
	}*/
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
	//強めの敵の描画
	for (StrongEnemy* strongEnemy : strongEnemys_) {
		strongEnemy->Draw(viewProjection_);
	}
	//反射する敵の描画
	for (ReflectEnemy* reflectEnemy : reflectEnemys_) {
		reflectEnemy->Draw(viewProjection_);
	}
	//曲がる敵の描画
	for (CurveEnemy* curveEnemy : curveEnemys_) {
		curveEnemy->Draw(viewProjection_);
	}

	// 天球の描画
	skydome_->Draw(viewProjection_);

	// ボーダーラインの描画
	borderline_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	playerHPSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheakAllCollisions() {
	// 対象物A(自機)とB(敵)
	Vector3 posA = {0};
	Vector3 posB = {0};

	// 2間点の距離
	float posAB;

	// 自キャラの半径
	float playerRadius = 3.0f;

#pragma region 自キャラと敵の当たり判定
	// 自キャラのワールド座標を取得
	posA = player_->GetWorldPosition();

	for (Enemy* enemy : enemys_) {


		// 敵のワールド座標を取得
		posB = enemy->GetWorldPosition();

		// 雑魚敵の半径
		float enemyBulletRadius = 4.0f;

		// AとBの距離を求める
		posAB = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y);
	   //(posB.z - posA.z) * (posB.z - posA.z);

		// 球と球との当たり判定
		if (posAB <= (playerRadius + enemyBulletRadius) * (playerRadius + enemyBulletRadius)) {
			//敵ダメージ判定を出す
			enemy->HitJudge(player_->GetAttackPow());

			player_->GetDamageAfter(enemy->AttackPlayer(player_->GetHP()));

			CheakHPCameraShake();
		} else {
			enemy->NotCollision();
			enemy->ResetFlag();
		}
	}

	for (StrongEnemy* strongEnemy : strongEnemys_) {
		// 敵のワールド座標を取得
		posB = strongEnemy->GetWorldPosition();

		// 雑魚敵の半径
		float enemyBulletRadius = 16.0f;

		// AとBの距離を求める
		posAB = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y);
		//(posB.z - posA.z) * (posB.z - posA.z);

		// 球と球との当たり判定
		if (posAB <= (playerRadius + enemyBulletRadius) * (playerRadius + enemyBulletRadius)) {
			// 敵ダメージ判定を出す
			strongEnemy->HitJudge(player_->GetAttackPow());
			// 敵から攻撃力を貰ってダメージを受ける
			//player_->Damage(strongEnemy->GetPower());
			
			player_->GetDamageAfter(strongEnemy->AttackPlayer(player_->GetHP()));

			// カメラの衝突判定
			CheakHPCameraShake();
			// 敵弾の衝突時コールバックを呼び出す
			// enemy->OnCollision();
		} else {
			strongEnemy->NotCollision();
			strongEnemy->ResetFlag();
			// ダメージを受けるフラグのリセット
			//player_->NotDamage();
		}
	}

	for (ReflectEnemy* reflectEnemy : reflectEnemys_) {
		// 敵のワールド座標を取得
		posB = reflectEnemy->GetWorldPosition();

		// 雑魚敵の半径
		float enemyBulletRadius = 6.0f;

		// AとBの距離を求める
		posAB = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y);
		//(posB.z - posA.z) * (posB.z - posA.z);

		// 球と球との当たり判定
		if (posAB <= (playerRadius + enemyBulletRadius) * (playerRadius + enemyBulletRadius)) {
			// 敵ダメージ判定を出す
			reflectEnemy->HitJudge(player_->GetAttackPow());

			player_->GetDamageAfter(reflectEnemy->AttackPlayer(player_->GetHP()));

			// カメラの衝突判定
			CheakHPCameraShake();
			// 敵弾の衝突時コールバックを呼び出す
			// enemy->OnCollision();
		} else {
			reflectEnemy->NotCollision();
			reflectEnemy->ResetFlag();
		}
	}

	for (CurveEnemy* curveEnemy : curveEnemys_) {
		// 敵のワールド座標を取得
		posB = curveEnemy->GetWorldPosition();

		// 雑魚敵の半径
		float enemyBulletRadius = 6.0f;

		// AとBの距離を求める
		posAB = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y);
		//(posB.z - posA.z) * (posB.z - posA.z);

		// 球と球との当たり判定
		if (posAB <= (playerRadius + enemyBulletRadius) * (playerRadius + enemyBulletRadius)) {
			// 敵ダメージ判定を出す
			curveEnemy->HitJudge(player_->GetAttackPow());

			player_->GetDamageAfter(curveEnemy->AttackPlayer(player_->GetHP()));

			// カメラの衝突判定
			CheakHPCameraShake();
			// 敵弾の衝突時コールバックを呼び出す
			// enemy->OnCollision();
		} else {
			curveEnemy->NotCollision();
			curveEnemy->ResetFlag();
		}
	}

}

void GameScene::CheakHPCameraShake() {
	for (Enemy* enemy : enemys_) {
		camera_->ShakeStart(enemy->GetHP());
	 }
	for (StrongEnemy* strongEnemy : strongEnemys_) {
		camera_->ShakeStart(strongEnemy->GetHP());
	}
	for (CurveEnemy* curveEnemy : curveEnemys_) {
		camera_->ShakeStart(curveEnemy->GetHP());
	}
	for (ReflectEnemy* reflectEnemy : reflectEnemys_) {
		camera_->ShakeStart(reflectEnemy->GetHP());
	}
	}


void GameScene::EnemySpawn(Vector3 position, Vector3 velocity) { 
	Enemy* enemy = new Enemy;
	
	enemy->Initialize(modelEnemy_.get(),position,velocity,viewProjection_);

	enemys_.push_back(enemy);
}

void GameScene::LoadEnemyPopData(const std::string& fileName) {
	enemyPopCommands.clear();

	// ファイルを開く
	std::ifstream file;
	file.open(fileName);
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (enemyPopWaitFlag) {
		enemyPopWaitTimer--;
		if (enemyPopWaitTimer <= 0) {
			// 待機完了
			enemyPopWaitFlag = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	/// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行文の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			EnemySpawn(Vector3(x, y, z), {0.0f, -0.15f, 0.0f});
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			enemyPopWaitFlag = true;
			enemyPopWaitTimer = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::StrongEnemySpawn(Vector3 position, Vector3 velocity) {
	StrongEnemy* strongEnemy = new StrongEnemy;

	strongEnemy->Initialize(modelStrongEnemy_.get(), position, velocity,viewProjection_);

	strongEnemys_.push_back(strongEnemy);
}

void GameScene::LoadStrongEnemyPopData(const std::string& fileName) {
	strongEnemyPopCommands.clear();

	// ファイルを開く
	std::ifstream file;
	file.open(fileName);
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	strongEnemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateStrongEnemyPopCommands() {
	// 待機処理
	if (strongEnemyPopWaitFlag) {
		strongEnemyPopWaitTimer--;
		if (strongEnemyPopWaitTimer <= 0) {
			// 待機完了
			strongEnemyPopWaitFlag = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	/// コマンド実行ループ
	while (getline(strongEnemyPopCommands, line)) {
		// 1行文の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			StrongEnemySpawn(Vector3(x, y, z), {0.0f, -0.05f, 0.0f});
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			strongEnemyPopWaitFlag = true;
			strongEnemyPopWaitTimer = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::ReflectEnemySpawn(Vector3 position, Vector3 velocity) {
	ReflectEnemy* reflectEnemy = new ReflectEnemy;

	reflectEnemy->Initialize(modelReflectEnemy_.get(), position, velocity, viewProjection_);

	reflectEnemys_.push_back(reflectEnemy);
}

void GameScene::LoadReflectEnemyPopData(const std::string& fileName) {
	reflectEnemyPopCommands.clear();

	// ファイルを開く
	std::ifstream file;
	file.open(fileName);
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	reflectEnemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateReflectEnemyPopCommands() {
	// 待機処理
	if (reflectEnemyPopWaitFlag) {
		reflectEnemyPopWaitTimer--;
		if (reflectEnemyPopWaitTimer <= 0) {
			// 待機完了
			reflectEnemyPopWaitFlag = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	/// コマンド実行ループ
	while (getline(reflectEnemyPopCommands, line)) {
		// 1行文の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			unsigned int currentTime = (int)time(nullptr);
			srand(currentTime);

			int r = (int)rand();

			int direction = r % 2;

			float velocity = 0.0f;
			const float kVelocity = 0.2f;

			if (direction == 0) {
				velocity = kVelocity;
			}

			if (direction == 1) {
				velocity = -kVelocity;
			}

			// 敵を発生させる
			ReflectEnemySpawn(Vector3(x, y, z), {velocity, -0.08f, 0.0f});
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			reflectEnemyPopWaitFlag = true;
			reflectEnemyPopWaitTimer = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::CurveEnemySpawn(Vector3 position, Vector3 velocity, bool directionFlag) {
	CurveEnemy* curveEnemy = new CurveEnemy;

	curveEnemy->Initialize(modelCurveEnemy_.get(), position, velocity, directionFlag,viewProjection_);

	curveEnemys_.push_back(curveEnemy);
}

void GameScene::LoadCurveEnemyPopData(const std::string& fileName) {
	reflectEnemyPopCommands.clear();

	// ファイルを開く
	std::ifstream file;
	file.open(fileName);
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	curveEnemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateCurveEnemyPopCommands() {
	// 待機処理
	if (curveEnemyPopWaitFlag) {
		curveEnemyPopWaitTimer--;
		if (curveEnemyPopWaitTimer <= 0) {
			// 待機完了
			curveEnemyPopWaitFlag = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	/// コマンド実行ループ
	while (getline(curveEnemyPopCommands, line)) {
		// 1行文の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			unsigned int currentTime = (int)time(nullptr);
			srand(currentTime);

			int r = (int)rand();

			int directionFlag = r % 2;

			// 敵を発生させる
			CurveEnemySpawn(Vector3(x, y, z), {0.0f, -0.2f, 0.0f}, directionFlag);
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			curveEnemyPopWaitFlag = true;
			curveEnemyPopWaitTimer = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::UpdateBorderLine() { 
	border_ -= 1;
	if (border_ <= 0) {
	
	}
}

void GameScene::Wave1Initialize() {
	LoadEnemyPopData("./Resources/enemyPop.csv");
	LoadStrongEnemyPopData("./Resources/strongEnemyPop.csv");
	LoadReflectEnemyPopData("./Resources/reflectEnemyPop.csv");
	LoadCurveEnemyPopData("./Resources/curveEnemyPop.csv");
}

void GameScene::Wave2Initialize() {
	LoadEnemyPopData("./Resources/enemyPop.csv");
	LoadStrongEnemyPopData("./Resources/strongEnemyPop.csv");
	LoadReflectEnemyPopData("./Resources/reflectEnemyPop.csv");
	LoadCurveEnemyPopData("./Resources/curveEnemyPop.csv");
}

void GameScene::WaveReset() { isWaveEnd = false; }

void GameScene::sceneReset() {
	//敵のリセット
	enemys_.clear();
	//シーン移行のリセット
	isSceneEnd = false;

}