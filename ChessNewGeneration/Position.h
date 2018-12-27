#pragma once
#include <string>
#include <array>
#include <vector>
#include <ostream>

class Position;
constexpr bool operator==(const Position &lhs, const Position &rhs) noexcept;
constexpr bool operator!=(const Position &lhs, const Position &rhs) noexcept;
class Position {
public:
	static const auto& getAllPossiblePositions() {
		static const std::array<const Position, 64> allPossiblePositions = {
			Position("A1"), Position("A2"), Position("A3"), Position("A4"), Position("A5"), Position("A6"), Position("A7"), Position("A8"),
			Position("B1"), Position("B2"), Position("B3"), Position("B4"), Position("B5"), Position("B6"), Position("B7"), Position("B8"),
			Position("C1"), Position("C2"), Position("C3"), Position("C4"), Position("C5"), Position("C6"), Position("C7"), Position("C8"),
			Position("D1"), Position("D2"), Position("D3"), Position("D4"), Position("D5"), Position("D6"), Position("D7"), Position("D8"),
			Position("E1"), Position("E2"), Position("E3"), Position("E4"), Position("E5"), Position("E6"), Position("E7"), Position("E8"),
			Position("F1"), Position("F2"), Position("F3"), Position("F4"), Position("F5"), Position("F6"), Position("F7"), Position("F8"),
			Position("G1"), Position("G2"), Position("G3"), Position("G4"), Position("G5"), Position("G6"), Position("G7"), Position("G8"),
			Position("H1"), Position("H2"), Position("H3"), Position("H4"), Position("H5"), Position("H6"), Position("H7"), Position("H8") };

		return allPossiblePositions;
	}

	static const auto& getAllColumns() noexcept {
		static const std::array<const std::string, 8> allColumn = { "A", "B", "C", "D", "E", "F", "G", "H" };
		return allColumn;
	}

	static const auto& getAllRows() noexcept {
		static const std::array<const std::string, 8> allRows = { "1", "2", "3", "4", "5", "6", "7", "8" };
		return allRows;
	}

private:
	constexpr bool isPositionValid() const noexcept {
		if (row_ < 0 || row_ >= getAllRows().size())
			return false;
		if (column_ < 0 || column_ >= getAllColumns().size())
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
