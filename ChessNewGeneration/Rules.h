#pragma once
#include "Pieces.h"
#include <typeindex>
#include <vector>
#include <algorithm>
#include <iostream>

class Rule {
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

class AttackRule {
public:
	static bool isAttackPossible(const Piece& attacker, const Piece& target, const std::vector<const Piece*>& pieces) {
		return attacker.getOwner() != target.getOwner() && attacker.isConsistentWithAttackRules(target.getPosition())
			&& CollisionRule::isThereNoCollisions(attacker.getRoute(target.getPosition()), pieces);
	}

	static bool isAttackValidMove(const Piece& attacker, const Piece& target, const std::vector<const Piece*>& pieces) {
		return typeid(target) != typeid(King) && isAttackPossible(attacker, target, pieces);
	}
};

class CheckRule {
public:
	static bool isThereNoCheck(const Player& playerToVerify, const std::vector<const Piece*>& pieces) {
		const auto& king = std::find_if(pieces.begin(), pieces.end(), [playerToVerify](const auto& piece) {
			return typeid(*piece) == typeid(King) && piece->getOwner() == playerToVerify;
		});

		const auto& check = std::find_if(pieces.begin(), pieces.end(), [king, &pieces, playerToVerify](const auto& piece) {
			return AttackRule::isAttackPossible(*piece, **king, pieces);
		});
	
		return check == std::end(pieces);
	}
};

class CheckMateRule {
public:
	static bool isThereCheckMate(const Player& playerToVerify, const std::vector<const Piece*>& pieces) {
		//get vec of all valid possible moves for white

		//foreach move verify isThereNoCheck()
			//if there is any move after whith isThereNoCheck returns true -> return false;

		return true;
	}
};
