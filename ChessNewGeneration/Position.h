#pragma once
#include <string>

class Position {
  static constexpr int MAX_NO = 7;
  static constexpr int MIN_NO = 0;
  constexpr bool isPositionValid() const noexcept {
    if (row_ < MIN_NO || row_ > MAX_NO)
      return false;
    if (column_ < MIN_NO || column_ > MAX_NO)
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
