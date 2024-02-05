#include "Borderline.h"
#include "ImGuiManager.h"

void Borderline::Initialize(Model* model) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {4.0f, 10.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, -70.0f, 130.0f};

	//耐久値を最大耐久値に合わせる
	lineEND = maxLineEND;
}

void Borderline::Update() {
	// 行列の更新
	worldTransform_.UpdateMatrix();
	
//#ifdef DEBUG
	ImGui::Begin("borderLine");
	ImGui::Text(
	    "lineEND:%d,maxlineEND:%d,breakLineFlag:%d",lineEND , maxLineEND, breakLineFlag);
	ImGui::End();

//#endif // DEBUG
}

void Borderline::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection); 
}

void Borderline::ReduceEND() { lineEND -= 1; }

void Borderline::ResetEND() { lineEND = maxLineEND; }

int Borderline::ReturnFlag() {
	if (lineEND <= 0) {
		return 1;
	}
	return 0; 
}
