#pragma once
#include "Pieces.h"
#include <typeindex>
#include <vector>
#include <algorithm>
#include <iostream>

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
	static bool isThereNoCollisions(const std::vector<Position>& route, const std::vector<const Piece*>& pieces) {
		for (const auto& position : route)
			for (const auto& piece : pieces)
				if (position == piece->getPosition())
					return false;
		return true;
	}
};

class CheckRule {
public:
	static bool isThereNoCheck(const Player& playerToVerify, const std::vector<const Piece*>& pieces) {
		const auto& king = std::find_if(pieces.begin(), pieces.end(), [playerToVerify](const auto& piece) {
			return typeid(*piece) == typeid(King) && piece->getOwner() == playerToVerify;
		});

		const auto& check = std::find_if(pieces.begin(), pieces.end(), [king, &pieces, playerToVerify](const auto& piece) {
			return piece->getOwner() != playerToVerify && piece->isConsistentWithAttackRules((*king)->getPosition()) &&
				CollisionRule::isThereNoCollisions(piece->getRoute((*king)->getPosition()), pieces);
		});

		return check == std::end(pieces);
	}
};
