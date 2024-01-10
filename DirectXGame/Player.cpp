#include"Player.h"
#include<cassert>
#include"ImGuiManager.h"

void Player::Initialize(Model* model, Vector3 position) {
	//NULLポインタチェック
	assert(model);
	//引数からデータを受け取る
	model_ = model;
	//ワールド変換の初期化
	worldTransform_.Initialize();
	//引数からワールドをもらう
	worldTransform_.translation_ = position;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update(ViewProjection& viewProjection) { 

	{
		//

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
		const float kDistanceTestObject = 100.0f;

		worldTransform_.translation_ = Add(posNear, Multiply(kDistanceTestObject, mouseDirection));

		////Z軸を固定化
		//worldTransform_.translation_.z = 0.0f;

		ImGui::Begin("Player");
		ImGui::Text(
		    "model:%f,%f,%f", worldTransform_.translation_.x,
			worldTransform_.translation_.y,
		    worldTransform_.translation_.z);
		ImGui::End();
	}
	

	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() {}
