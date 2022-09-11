#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "DataDictionary.h"

using namespace vxe::med;

void DataDictionary::LoadDERegistry() {
	std::ifstream rfile(_DERegistryPath);
	std::stringstream rstream;
	std::string line;
	rstream << rfile.rdbuf();

	while (std::getline(rstream, line)) {
		std::istringstream linestream(line);
		std::string csventry;

		tag_t Tag;
		std::string name;
		uint16_t VR;
		size_t VM;

		std::getline(linestream, csventry, ',');
		Tag = std::stoul(csventry, nullptr, 16);

		std::getline(linestream, csventry, ',');
		name.assign(csventry, 1);
		std::getline(linestream, csventry, ',');

		VR = 0;
		if (csventry.length() >= 2 && csventry.find("or", 3) == std::string::npos) {
			VR += (uint16_t)(csventry[1]);
			VR = VR << 8;
			VR += (uint16_t)(csventry[2]);
		}

		std::getline(linestream, csventry, ',');
		// VM.assign(csventry, 1);
		if (csventry.find("-") != std::string::npos ||
			csventry.find("n")!= std::string::npos ||
			(csventry.find("or") != std::string::npos)) {
			VM = 0;
		} else {
			VM = (size_t)std::stoull(csventry);
		}

		_DERegistry.emplace(Tag, DEInfo{Tag, name, VR, VM });

		linestream.clear();
	}
}

void DataDictionary::LoadDEVerboseRegistry() {
	std::ifstream rfile(_DERegistryPath);
	std::stringstream rstream;
	std::string line;
	rstream << rfile.rdbuf();

	while (std::getline(rstream, line)) {
		std::istringstream linestream(line);
		std::string csventry;

		tag_t Tag;
		std::string name;
		std::string VR;
		std::string VM;

		std::getline(linestream, csventry, ',');
		Tag = std::stoul(csventry, nullptr, 16);

		std::getline(linestream, csventry, ',');
		name.assign(csventry, 1);
		std::getline(linestream, csventry, ',');
		VR.assign(csventry, 1);
		std::getline(linestream, csventry, ',');
		VM.assign(csventry, 1);

		_DEVerboseRegistry.emplace(Tag, DEInfoVerbose{Tag, name, VR, VM });

		linestream.clear();
	}
}