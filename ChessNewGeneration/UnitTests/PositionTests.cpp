#include "pch.h"
#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#include "../Position.h"
TEST(PositionTests, StingTranslationValid) {
	Position position1("A4");
	EXPECT_EQ(position1.column_, 0);
	EXPECT_EQ(position1.row_, 3);

	Position position2("H1");
	EXPECT_EQ(position2.column_, 7);
	EXPECT_EQ(position2.row_, 0);

	Position position3("D3");
	EXPECT_EQ(position3.column_, 3);
	EXPECT_EQ(position3.row_, 2);

	Position position4("G8");
	EXPECT_EQ(position4.column_, 6);
	EXPECT_EQ(position4.row_, 7);

	Position position5("C1");
	EXPECT_EQ(position5.column_, 2);
	EXPECT_EQ(position5.row_, 0);
}
TEST(PositionTests, StingTranslationInvalid) {
	EXPECT_ANY_THROW(Position("ABC"));
	EXPECT_ANY_THROW(Position("A"));
	EXPECT_ANY_THROW(Position(""));
	EXPECT_ANY_THROW(Position("A9"));
	EXPECT_ANY_THROW(Position("12"));
	EXPECT_ANY_THROW(Position("I2"));
	EXPECT_ANY_THROW(Position("I16"));
	EXPECT_ANY_THROW(Position("-1"));
}
TEST(PositionTests, PositionEqualOperator) {
	EXPECT_TRUE(Position("A1") == Position(0, 0));
	EXPECT_TRUE(Position("H8") == Position(7, 7));
	EXPECT_TRUE(Position("D4") == Position(3, 3));
	EXPECT_TRUE(Position("E6") == Position(4, 5));
	EXPECT_TRUE(Position("F2") == Position(5, 1));
	EXPECT_TRUE(Position("B3") == Position(1, 2));

	EXPECT_FALSE(Position("A2") == Position(0, 2));
	EXPECT_FALSE(Position("B1") == Position(1, 1));
	EXPECT_FALSE(Position("H8") == Position(7, 6));
	EXPECT_FALSE(Position("C2") == Position(3, 2));
	EXPECT_FALSE(Position("A1") == Position(1, 1));

	EXPECT_TRUE(Position("A2") == Position(0, 1));
	EXPECT_TRUE(!(Position("A2") != Position(0, 1)));
}