#include "PlayerUpgrade.h"
#include "Player.h"

void PlayerUpgrade::AttackUpgrade(int attackPower) {
	attackPower += attack_; 
}

void PlayerUpgrade::HitPointUpgrade(int playerHP) { 
	playerHP += hitPoint_;
}

void PlayerUpgrade::RecoveryUpgrade(int recovery) { 
	recovery += recovery_; 
}
