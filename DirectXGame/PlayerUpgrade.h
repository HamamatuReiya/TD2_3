#pragma once

class PlayerUpgrade {
public:
	//強化内容
	//攻撃力
	void AttackUpgrade(int attackPower);
	//体力上昇
	void HitPointUpgrade(int playerHP);
	//回復力の上昇
	void RecoveryUpgrade(int recovery);

private:
	//加算する攻撃力
	int attack_ = 1;
	//体力
	int hitPoint_ = 2;
	//回復力
	int recovery_ = 1;
};
