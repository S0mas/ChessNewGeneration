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
	bool eval(const std::vector<Position>& route, const std::vector<Piece*>& pieces) {
		for (const auto& position : route)
			for (const auto& piece : pieces)
				if (position == piece->getPosition())
					return false;
		return true;
	}
};
