#pragma once
#include "Pieces.h"
#include <typeindex>
#include <vector>

class Rule {
	
};

class AttackRule {
public:
	bool eval(const Piece& attacker, const Piece& target) {
		if (typeid(target) == typeid(King) || attacker.getOwner() == target.getOwner())
			return false;
		return attacker.isConsistentWithAttackRules(target.getPosition());
	}
};

class CollisionRule {
public:
	bool eval(const Position& destination, const std::vector<Piece*>& pieces) {
		return true;
	}
};

