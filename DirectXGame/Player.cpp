#include"Player.h"
#include<cassert>
#include"ImGuiManager.h"

void Player::Initialize(Model* model, Model* model2, Vector3 position) {
	//NULLポインタチェック
	assert(model);
	//引数からデータを受け取る
	model_ = model;
	model2_ = model2;
	//スケールの設定
	worldTransform_.scale_ = {2.0f, 2.0f, 2.0f};

	//ワールド変換の初期化
	worldTransform_.Initialize();
	//引数からワールドをもらう
	worldTransform_.translation_ = position;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();

}

void Player::Update(ViewProjection& viewProjection) { 

	switch (playerState) { 
	case PlayerState::isAlive:

		//生きているときはフラグをTRUE
		isDead_ = false;

		Move(viewProjection);

		#ifdef DEBUG
		
		if (input_->IsTriggerMouse(1) == 1) {
			playerHP_ = 0.0f;
		}

         #endif // DEBUG

		
		// HP0の処理
		if (playerHP_ <= 0.0f) {
			playerHP_ = 0.0f;
		}
		
		worldTransform_.UpdateMatrix();

		//体力が0になったら状態を切り替える
		if (playerHP_ <= 0) {
			playerState = PlayerState::isDead;
		}

		break;

		case PlayerState::isDead:

		//死んでいるときはフラグをFALSE
		isDead_ = true;

		Recovery();

		break;
	}

	
}

void Player::Draw(ViewProjection& viewProjection) {
	if (isDead_ == true) {
	model2_->Draw(worldTransform_, viewProjection); 
	} else {
	model_->Draw(worldTransform_, viewProjection); 
	}
}

void Player::Move(ViewProjection& viewProjection) {

		{
		// 自機の現在座標を取得
		Vector2 playerPosition = {worldTransform_.translation_.x, worldTransform_.translation_.y};

		POINT mousePosition;
		// マウス座標(スクリーン座標)を計算する
		GetCursorPos(&mousePosition);

		// クライアントエリアに座標を変換する
		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePosition);

		// ビューポート行列
		Matrix4x4 matViewport2 =
		    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		// ビュープロジェクションビューポート合成行列
		Matrix4x4 matVPV =
		    Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport2);

		// 合成行列の逆行列を計算する
		Matrix4x4 matInverseVPV = Inverse(matVPV);

		// スクリーン座標
		Vector3 posNear = Vector3((float)mousePosition.x, (float)mousePosition.y, 0);
		Vector3 posFar = Vector3((float)mousePosition.x, (float)mousePosition.y, 1);

		// スクリーン座標系からワールド座標系へ
		posNear = Transform(posNear, matInverseVPV);
		posFar = Transform(posFar, matInverseVPV);

		// マウス例の方向
		Vector3 mouseVector = Subtract(posFar, posNear);
		Vector3 mouseDirection = Add(posNear, mouseVector);

		mouseDirection = Normalize(mouseDirection);

		// カメラから照準オブジェクトの距離
		const float kDistanceTestObject = 200.0f;

		worldTransform_.translation_ = Add(posNear, Multiply(kDistanceTestObject, mouseDirection));

		////Z軸を固定化
		worldTransform_.translation_.z = 130.0f;

		#ifdef DEBUG

		ImGui::Begin("Player");
		ImGui::Text(
		    "model:%f,%f,%f,HP:%f,\nnormal:%d", worldTransform_.translation_.x,
		    worldTransform_.translation_.y, worldTransform_.translation_.z, playerHP_, damageFlag_);

		ImGui::Text("model:%f,%f,%f", mouseDirection.x, mouseDirection.y, mouseDirection.z);
		ImGui::End();

#endif // DEBUG

	}
}

void Player::Recovery() { 
	if (input_->IsTriggerMouse(0) == 1) {
		playerHP_ += recoveryPower;
	}
	if (playerHP_ >= kPlayerHP_) {
		playerHP_ = kPlayerHP_;
		playerState = PlayerState::isAlive;
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::GetDamageAfter(float hp) { playerHP_ = hp; }

