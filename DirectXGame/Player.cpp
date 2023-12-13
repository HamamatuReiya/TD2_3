#include"Player.h"
#include<cassert>

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

void Player::Update() {

}

void Player::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }