#include "PlayerUpgrade.h"
#include "Player.h"

void PlayerUpgrade::AttackUpgrade(float attackPower) {
	attackPower += attack_; 
}

void PlayerUpgrade::HitPointUpgrade(float playerHP) { 
	playerHP += hitPoint_;
}

void PlayerUpgrade::RecoveryUpgrade(float recovery) { 
	recovery += recovery_; 
}
