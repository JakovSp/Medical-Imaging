/*
DataDictionaryTest.cpp
	
	Tests:
		*Registry Format
*/
#pragma once
#include <gtest/gtest.h>
#include <gtest/gtest-matchers.h>
#include <gmock/gmock.h>
#include <iostream>
#include <fstream>
#include <Read/Registry/DataDictionary.h>

using namespace vxe::med;

TEST(DataDictSuite, ReadRegistry) {
	std::ifstream rfile("DataElementRegistry.csv");
	std::stringstream rstream;
	std::string line;
	rstream << rfile.rdbuf();

	EXPECT_TRUE(rstream.good());

	// std::map<tag_t, DEInfoVerbose> DERegistry;

	size_t linenum = 1;
	while (std::getline(rstream, line)) {
		std::istringstream linestream(line);
		std::string csventry;

		tag_t Tag = 0;
		std::string name(0, '\0');
		std::string VR(0, '\0');
		std::string VM(0, '\0');

		std::getline(linestream, csventry, ',');
		Tag = std::stoul(csventry, nullptr, 16);
		EXPECT_GT(Tag, 0) << "at line " << linenum;
		EXPECT_LT(Tag, 0xFFFFFFFF) << "at line " << linenum;

		std::getline(linestream, csventry, ',');
		name.assign(csventry, 1);
		EXPECT_LT(name.size(), name.find(" ")) << "at line " << linenum;

		std::getline(linestream, csventry, ',');
		VR.assign(csventry, 1);
		if ((Tag != Item) && (Tag != ItemDelimitationItem) && (Tag != SequenceDelimitationItem)) {
			EXPECT_TRUE(VR.size()) << "at line " << linenum;
			EXPECT_TRUE(std::isupper(VR[0])) << "at line " << linenum;
			EXPECT_TRUE(std::isupper(VR[1])) << "at line " << linenum;
			if (VR.size() > 2) { EXPECT_EQ(' ', VR[2]) << "at line " << linenum; }
		}

		std::getline(linestream, csventry, ',');
		VM.assign(csventry, 1);
		if (VM.size() == 1) { EXPECT_TRUE(std::isalnum(VM[0])) << "at line " << linenum; }
		else if (VM.size() >= 3) { EXPECT_EQ('-', VM[1]) << "at line " << linenum; }

		// DERegistry.emplace(Tag, DEInfoVerbose{Tag, name, VR, VM });

		linestream.clear();
		linenum++;
	}

}

