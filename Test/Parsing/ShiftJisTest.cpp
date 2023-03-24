#include "CppSlippi/Parsing/ShiftJis.h"

#include <gtest/gtest.h>


namespace cpp_slippi::parsing {

TEST(ShiftJisTest, TestAscii) {
	EXPECT_EQ(shiftJisToUtf8("Hello, world", false), "Hello, world");
}

TEST(ShiftJisTest, TestKana) {
	EXPECT_EQ(shiftJisToUtf8("\x82\xA0\x82\xA2\x82\xA4", false), "あいう");
}

TEST(ShiftJisTest, TestKanji) {
	EXPECT_EQ(shiftJisToUtf8("\x8A\x64\xE9\xD5\xE4\x82", false), "嚇鰈艪");
}

TEST(ShiftJisTest, TestMixed) {
	EXPECT_EQ(shiftJisToUtf8("Hello, \x8A\x64\xE9\xD5\xE4\x82! 4\x82\xA0.", false), "Hello, 嚇鰈艪! 4あ.");
}

TEST(ShiftJisTest, TextFullwidthAscii) {
	EXPECT_EQ(
		shiftJisToUtf8(
			"\x82\x67\x82\x85\x82\x8C\x82\x8C\x82\x8F\x81\x43\x81\x40\x82\x76\x82\x8F\x82\x92\x82\x8C\x82\x84\x81\x49",
			false),
		"Ｈｅｌｌｏ，　Ｗｏｒｌｄ！");
}

TEST(ShiftJisTest, TextFullwidthAsciiToHalfwidth) {
	EXPECT_EQ(
		shiftJisToUtf8(
			"\x82\x67\x82\x85\x82\x8C\x82\x8C\x82\x8F\x81\x43\x81\x40\x82\x76\x82\x8F\x82\x92\x82\x8C\x82\x84\x81\x49",
			true),
		"Hello, World!");
}

}  // namespace cpp_slippi::parsing
