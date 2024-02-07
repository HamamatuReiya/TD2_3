#pragma once

class PlayerUpgrade {
public:
	//強化内容
	//攻撃力
	void AttackUpgrade(int attackPower);
	//体力上昇
	void HitPointUpgrade(float playerHP);
	//回復力の上昇
	void RecoveryUpgrade(float recovery);

private:
	//加算する攻撃力
	int attack_ = 1;
	//体力
	float hitPoint_ = 30;
	//回復力
	float recovery_ = 5;
};
