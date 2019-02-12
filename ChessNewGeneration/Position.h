#pragma once
#include <string>
#include <array>
#include <vector>
#include <ostream>

class Position;
constexpr bool operator==(const Position &lhs, const Position &rhs) noexcept;
constexpr bool operator!=(const Position &lhs, const Position &rhs) noexcept;
class Position {
	constexpr bool isPositionValid() const noexcept {
		if (row_ < 0 || row_ >= rowsNo)
			return false;
		if (column_ < 0 || column_ >= columnsNo)
			return false;
		return true;
	}

	static auto stringToPosition(const std::string &str) {
		if (str.size() != 2)
			throw InvalidPositionStringSize();
		const auto &columnLetter = toupper(str[0]);
		const auto &rowNumber = str[1];
		return Position(columnLetter - 'A', rowNumber - '0' - 1);
	}
public:
	static constexpr int columnsNo = 8;
	static constexpr int rowsNo = 8;
	int column_{};
	int row_{};

	constexpr Position(const int c, const int r) : column_(c), row_(r) {
		if (!isPositionValid())
			throw InvalidPositionException();
	}

	explicit Position(const std::string &str) { *this = stringToPosition(str); }
	Position(const Position &original) noexcept = default;
	Position(Position &&original) noexcept = default;
	~Position() = default;
	Position &operator=(const Position &original) noexcept = default;
	Position &operator=(Position &&original) noexcept = default;
	Position &operator=(std::string &&str) {
		*this = stringToPosition(str);
		return *this;
	}

	std::string toString() const noexcept {
		std::string result("AA");
		result[0] = static_cast<char>(column_ + 'A');
		result[1] = static_cast<char>(row_ + '0' + 1);
		return result;
	}

	auto getSimplestRoute(const Position& destination) const noexcept {
		std::vector<Position> route;
		auto stepColumn = 1;
		auto stepRow = 1;
		if (column_ > destination.column_)
			stepColumn = -1;
		if (row_ > destination.row_)
			stepRow = -1;
		Position start = *this;
		while (start != destination) {
			if (start.column_ != destination.column_)
				start.column_ += stepColumn;
			if (start.row_ != destination.row_)
				start.row_ += stepRow;
			route.push_back(start);
		}
		route.pop_back();
		return route;
	}

	bool isDestinationPromotionRow() const noexcept {
		return row_ == 7 || row_ == 0;
	}

	class InvalidPositionException final : public std::exception {
		const char *what() const noexcept override {
			return "Invalid position specified";
		}
	};

	class InvalidPositionStringSize final : public std::exception {
		const char *what() const noexcept override {
			return "Invalid position string size, should be 2";
		}
	};
};

constexpr bool operator==(const Position &lhs, const Position &rhs) noexcept {
	return lhs.column_ == rhs.column_ && lhs.row_ == rhs.row_;
}

constexpr bool operator!=(const Position &lhs, const Position &rhs) noexcept {
	return !(lhs == rhs);
}

inline std::ostream& operator<<(std::ostream& out, const Position& position) noexcept {
	out << position.toString() << std::endl;
	return out;
}

class Board {
	static constexpr std::array<const char, 8> allColumns = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
	static constexpr std::array<const char, 8> allRows = { '1', '2', '3', '4', '5', '6', '7', '8' };
	static constexpr std::array<const Position, Position::columnsNo*Position::rowsNo> allPossiblePositions = {
		Position(0,0), Position(1,0), Position(2,0), Position(3,0), Position(4,0), Position(5,0), Position(6,0), Position(7,0),
		Position(0,1), Position(1,1), Position(2,1), Position(3,1), Position(4,1), Position(5,1), Position(6,1), Position(7,1),
		Position(0,2), Position(1,2), Position(2,2), Position(3,2), Position(4,2), Position(5,2), Position(6,2), Position(7,2),
		Position(0,3), Position(1,3), Position(2,3), Position(3,3), Position(4,3), Position(5,3), Position(6,3), Position(7,3),
		Position(0,4), Position(1,4), Position(2,4), Position(3,4), Position(4,4), Position(5,4), Position(6,4), Position(7,4),
		Position(0,5), Position(1,5), Position(2,5), Position(3,5), Position(4,5), Position(5,5), Position(6,5), Position(7,5),
		Position(0,6), Position(1,6), Position(2,6), Position(3,6), Position(4,6), Position(5,6), Position(6,6), Position(7,6),
		Position(0,7), Position(1,7), Position(2,7), Position(3,7), Position(4,7), Position(5,7), Position(6,7), Position(7,7) };
public:
	static constexpr const std::array<const Position, Position::columnsNo*Position::rowsNo>& getAllPossiblePositions() noexcept  {
		return allPossiblePositions;
	}

	static constexpr const auto& getAllColumns() noexcept {
		return allColumns;
	}

	static constexpr const auto& getAllRows() noexcept {
		return allRows;
	}
};