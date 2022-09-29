#include <iostream>
// #include <gtest/gtest.h>

#include "pch.h"
#include "Reader.h"
#include <Convert/Converter.h>

using namespace std;
using namespace vxe::med;

bool DICOMReader::IsDICOMDIR(DataSet& MetaInfo) {
	const std::string classUID = MetaInfo[MediaStorageSOPClassUID].FetchValue<std::string>();
	if (!MetaInfo[MediaStorageSOPClassUID].FetchValue<std::string>().compare(MediaStorageDirectoryStorage))
		return true;

	return false;
}

void DICOMReader::Read(std::filesystem::path dirname) {
	if (dirname.empty())
		dirname = std::filesystem::current_path().append(_inputdirpath.c_str());
	ReadDirectory(dirname);
	ConstructFileSet();
}

void DICOMReader::ReadFile(std::filesystem::path file) {
	_parsers.push_back(DICOMParser{ file, &dict });
	if (!_parsers.back().IsDICOM()) {
		return;
	}
	DICOMInstance newInstance;
	newInstance.MetaInfo = _parsers.back().ReadMetaInfo();
	newInstance.MainDataSet = _parsers.back().ReadMainDataSet(std::byte{0});
	if (IsDICOMDIR(newInstance.MetaInfo)) {
		DICOMDIR = newInstance;
	} else{
		_instances.push_back(newInstance);
	}
}

void DICOMReader::ReadDirectory(std::filesystem::path dirname) {
	for (const auto& entry : fs::directory_iterator(dirname)) {
		if (entry.is_directory()) {
			ReadDirectory(entry);
		} else {
			ReadFile(entry);
		}
	}
}

void DICOMReader::ConstructFileSet() {
	for (int i = 0; i < _instances.size(); i++) {
		MainFileSet.InsertNewInstance(_instances[i]);
	}
}


// int main() {
// 
// 	cout << __func__ << "() called." << endl;
// 	// input is a directory
// 	DICOMReader reader("Ankle");
// 	reader.Read();
// 
// 	DICOMConverter converter("output", reader.MainFileSet);
// 	converter.GatherVolumes();
// 	converter.Convert();
// 
// 	// choose a fixed value for MRI
// 	// for CT show mapping Hounsfield's scale to the image
// 	// Hounsfield scale is a map of tissue to ranges
// 
// 	// Writer writer("newimage.bmp");
// 	// writer.WriteBMP(reader._instances[0], "newimage.bmp");
// 	cout << __func__ << "() returning." << endl;
// 
// 	return 0;
// }
