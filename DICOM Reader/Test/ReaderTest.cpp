/*
ReaderTest.cpp

	Tests:
		* File initialization
		* Loading a file using std streams
*/
#pragma once
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#include <map>
#include <Read/Reader.h>

using namespace vxe::med;

class FileTest : public testing::TestWithParam<const char*> {
	std::filesystem::path inputfile;

	void SetUp() override{
	}
	void TearDown() override{
		inputfile.clear();
	}
};

TEST_P(FileTest, OpenFile) {
	std::ifstream inputfilestream(GetParam());
	ASSERT_FALSE(inputfilestream.fail());
	EXPECT_TRUE(inputfilestream.is_open());
}

INSTANTIATE_TEST_SUITE_P(PrefixOpenFile, FileTest, testing::Values("Reader.cpp"));

class DICOMReaderTest : public ::testing::Test {
public:

	DICOMReader reader;
	std::filesystem::path DERegistryPath;
	void SetUp() override {
		// reader.LoadNewFile("MRBRAIN.DCM");
		DERegistryPath = "DataElementRegistry.dreg";
	}
	void TearDown() override {
	}
};

TEST(DICOMReaderTest, ReadFileBinary) {
	std::ifstream rfile("MRBRAIN.DCM", std::ios::binary | std::ios::ate);
	ASSERT_TRUE(rfile.good());

	std::streamsize size = rfile.tellg();
	ASSERT_LT(0, size);

	rfile.seekg(0, std::ios::beg);
	std::vector<char> buffer(size);
	rfile.read(buffer.data(), size);
	EXPECT_EQ(size, buffer.size());
}
