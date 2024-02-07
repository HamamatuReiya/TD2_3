#include "PlayerUpgrade.h"
#include "Player.h"

void PlayerUpgrade::AttackUpgrade(int attackPower) {
	attackPower += attack_; 
}

void PlayerUpgrade::HitPointUpgrade(float playerHP) { 
	playerHP += hitPoint_;
}

void PlayerUpgrade::RecoveryUpgrade(float recovery) { 
	recovery += recovery_; 
}
