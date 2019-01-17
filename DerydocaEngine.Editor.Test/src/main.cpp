#include "EditorTestPch.h"

int main(int argc, char **argv) {
	glewInit();
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}