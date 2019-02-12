#pragma once
#include "Chessboard.h"

class Command {
	Piece* subject;
public:
	virtual ~Command() = default;
	virtual bool checkPreConditions(const ChessBoard& chessboard) = 0;
	virtual void execute(ChessBoard&) = 0;
};


class DefaultMove : public Command {
	Piece* target;
public:
	bool checkPreConditions(const ChessBoard& chessboard) override {
		return false;
	}

	void execute(ChessBoard&) override {
		return;
	}
};

class Castling : public Command {
public:
	bool checkPreConditions(const ChessBoard& chessboard) override {
		return false;
	}

	void execute(ChessBoard&) override {
		return;
	}
};

class EnPassant : public Command {
public:
	bool checkPreConditions(const ChessBoard& chessboard) override {
		return false;
	}

	void execute(ChessBoard&) override {
		return;
	}
};

class Promotion : public Command {
public:
	bool checkPreConditions(const ChessBoard& chessboard) override {
		return false;
	}

	void execute(ChessBoard&) override {
		return;
	}
};