#pragma once

class PlayerUpgrade {
public:

	void Initialize();
	//強化内容
	//攻撃力
	int AttackUpgrade(int attackPower);
	//体力上昇
	float HitPointUpgrade(float playerHP);
	//回復力の上昇
	float RecoveryUpgrade(float recovery);

private:
	//加算する攻撃力
	int attack_=0;
	//体力
	float hitPoint_=0.0f;
	//回復力
	float recovery_=0.0f;
};
