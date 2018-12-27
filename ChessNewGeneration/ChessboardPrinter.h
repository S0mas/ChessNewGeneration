#pragma once
#include <array>
#include <string>

enum class Colour {
	White = 0,
	Black = 1
};

struct Field {
	int length = 3;
	int hight = 3;

	std::string spaceChar = " ";
	std::string piece_ = " ";
};

struct Row {
	int id_;
	std::array<Field, 8> fields;
	Row(const int id) : id_(id) {
		const auto rest = id % 2;
		for (auto i = 0; i < 8; ++i) {
			if (i % 2 != rest)
				fields[i].spaceChar = "#";
		}
	}

	std::string toString() {
		std::string result;
		result += " ";
		for (const auto& field : fields) {
			for (auto i = 0; i < field.length; ++i)
				result += field.spaceChar;
		}
		result += "\n";
		result += std::to_string(id_);
		for (const auto& field : fields) {
			result += field.spaceChar;
			result += field.piece_;
			result += field.spaceChar;
		}
		result += "\n";
		result += " ";
		for (const auto& field : fields) {
			for (auto i = 0; i < field.length; ++i)
				result += field.spaceChar;
		}
		result += "\n";

		return result;
	}
};

