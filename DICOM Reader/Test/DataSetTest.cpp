/*
DataSetTest.cpp

	Tests:
		* Conversions from strings to integral and floating point types
		* Heterogeneous container representing a runtime Data Set
*/
#include <gtest/gtest.h>
#include <Read/DataSet.h>

// TEST(DataSetTests, InsertNewElements) {
// 	DataSet RootSet;
// 
// 	std::string strbuffer("16abcdef");
// 	std::vector<char> binbuffer(strbuffer.begin(), strbuffer.end());
// 
// 	DataElement ShortDE;
// 	uint16_t shortvalue = std::stoi(binbuffer.data());
// 	ShortDE.data.push_back(shortvalue);
// 	// ShortDE.data.back().emplace<uint16_t>(shortvalue);
// 	// ShortDE.data.emplace<uint16_t>(&shortvalue);
// 	RootSet.elements.push_back(ShortDE);
// 
// 	uint16_t testvalue = std::get<uint16_t>(ShortDE.data.back());
// 	EXPECT_EQ(16, testvalue);
// 
// 	DataElement StringsDE;
// 	std::string str(binbuffer.data(), binbuffer.data() + 2);
// 	// StringsDE.data.back().emplace<std::string>(str);
// 	StringsDE.data.push_back(str);
// 	RootSet.elements.push_back(StringsDE);
// 
// 	std::string teststring = std::get<std::string>(StringsDE.data.back());
// 	EXPECT_EQ("16", teststring);
// 	
// 	EXPECT_EQ(2,RootSet.elements.size());
// }
// 
// TEST(DataSetTests, Convert) {
// }
