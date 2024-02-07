#include "PlayerUpgrade.h"
#include "Player.h"

void PlayerUpgrade::Initialize() {
	// 加算する攻撃力
	attack_ = 1;
	// 体力
	hitPoint_ = 30;
	// 回復力
	recovery_ = 5;
}

int PlayerUpgrade::AttackUpgrade(int attackPower) {
	attackPower += attack_; 
	return attackPower;
}

float PlayerUpgrade::HitPointUpgrade(float playerHP) { 
	playerHP += hitPoint_;
	return playerHP;
}

float PlayerUpgrade::RecoveryUpgrade(float recovery) { 
	recovery += recovery_; 
	return recovery;
}
