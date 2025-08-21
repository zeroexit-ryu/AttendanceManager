#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>

#include "gmock/gmock.h"
#include "attendance.h"

using namespace std;



int main(int argc, char* argv[]) {
#ifdef _DEBUG
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	DataProcessor dp;
	dp.processData();
	return 0;
#endif
}
