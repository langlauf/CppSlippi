#include "CppSlippi/Parsing/ShiftJis.h"

#include <gtest/gtest.h>


namespace cpp_slippi::parsing {

TEST(ShiftJisTest, TestAscii) {
	ShiftJisConverter shiftJis(false);
	EXPECT_EQ(shiftJis.toUtf8("Hello, world"), "Hello, world");
}

TEST(ShiftJisTest, TestKana) {
	ShiftJisConverter shiftJis(false);
	EXPECT_EQ(shiftJis.toUtf8("\x82\xA0\x82\xA2\x82\xA4"), "あいう");
}

TEST(ShiftJisTest, TestKanji) {
	ShiftJisConverter shiftJis(false);
	EXPECT_EQ(shiftJis.toUtf8("\x8A\x64\xE9\xD5\xE4\x82"), "嚇鰈艪");
}

TEST(ShiftJisTest, TestMixed) {
	ShiftJisConverter shiftJis(false);
	EXPECT_EQ(shiftJis.toUtf8("Hello, \x8A\x64\xE9\xD5\xE4\x82! 4\x82\xA0."), "Hello, 嚇鰈艪! 4あ.");
}

TEST(ShiftJisTest, TextFullwidthAscii) {
	ShiftJisConverter shiftJis(false);
	EXPECT_EQ(
		shiftJis.toUtf8(
			"\x82\x67\x82\x85\x82\x8C\x82\x8C\x82\x8F\x81\x43\x81\x40\x82\x76\x82\x8F\x82\x92\x82\x8C\x82\x84\x81\x49"),
		"Ｈｅｌｌｏ，　Ｗｏｒｌｄ！");
}

TEST(ShiftJisTest, TextFullwidthAsciiToHalfwidth) {
	ShiftJisConverter shiftJis(true);
	EXPECT_EQ(
		shiftJis.toUtf8(
			"\x82\x67\x82\x85\x82\x8C\x82\x8C\x82\x8F\x81\x43\x81\x40\x82\x76\x82\x8F\x82\x92\x82\x8C\x82\x84\x81\x49"),
		"Hello, World!");
}

}  // namespace cpp_slippi::parsing
