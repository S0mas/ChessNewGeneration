#pragma once
#include "pch.h"
#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#include <gtest/gtest.h>
#include <isolator.h>
#include "../ChessNewGeneration/Piece.h"

class PieceTests : public ::testing::Test {
public:
	void TearDown() final {
		ISOLATOR_CLEANUP();
	}
};
