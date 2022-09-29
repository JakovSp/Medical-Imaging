#pragma once
#include <filesystem>
#include <iostream>
#include <fstream>
#include <variant>
#include <string>

#include "pch.h"

#include "Registry/UIDRegistry.h"
#include "Registry/DataDictionary.h"
#include "ValueRepresentation.h"

#include "DataSet.h"
#include "DataElement.h"

namespace vxe::med {
	class DICOMParser {
	public:
		DICOMParser() : _inputfilepath(""), _dict(nullptr), _pos(0) {}
		DICOMParser(std::string filename, DataDictionary* dict) : _inputfilepath(filename), _dict(dict), _pos(0) {
			LoadNewFile(filename);
		}
		DICOMParser(std::filesystem::path filename, DataDictionary* dict) : _inputfilepath(filename), _dict(dict), _pos(0) {
			LoadNewFile(filename);
		}

		template<typename T>
		void ReadBinary(DataElement& element);
		void ReadString(DataElement& element);
		template<typename T>
		void ReadNumericString(DataElement& element);
		template<typename T>
		void ReadStream(DataElement& element);
		void ReadSequence(DataElement& element);

		void ReadValue(DataElement& element);
		DataElement ReadDataElement(DataSet& RootSet);

		DataSet ReadMetaInfo();
		DataSet ReadMainDataSet(TransferSyntax TS);

		void LoadNewFile(std::filesystem::path filename);
		bool IsDICOM();
		bool IsDICOMDIR(DataSet& MetaInfo);

	private:
		std::filesystem::path _inputfilepath;
		std::vector<uint8_t> _filebuffer;
		std::size_t _pos;
		DataDictionary* _dict;
	};
}
