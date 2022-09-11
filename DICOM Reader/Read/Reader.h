/*
Reader.h

	Handles input and initializes a Parser
*/
#pragma once
#include <filesystem>
#include <iostream>
#include <fstream>
#include <variant>
#include <string>
#include <filesystem>

#include "Registry/DataDictionary.h"
#include "ValueRepresentation.h"
#include "DataElement.h"
#include "FileSet.h"
#include "Parser.h"

namespace fs = std::filesystem;

namespace vxe::med {
	class DICOMReader{
	public:
		DICOMReader() : _inputdirpath("") {
			dict.LoadDERegistry();
			Read();
		}
		DICOMReader(std::wstring dirname) : _inputdirpath(dirname) {
			dict.LoadDERegistry();
			Read();
		}

		bool IsDICOMDIR(DataSet& MetaInfo);
		void Read(std::filesystem::path dirname = L"");
		void ReadFile(std::filesystem::path file);
		void ReadDirectory(std::filesystem::path dirname);
		void ConstructFileSet();

	public:
		FileSet MainFileSet;
		std::vector<DICOMInstance> _instances;
	private:
		std::filesystem::path _inputdirpath;
		std::vector<DICOMParser> _parsers;
		DICOMInstance DICOMDIR;
		DataDictionary dict;
	};
}
