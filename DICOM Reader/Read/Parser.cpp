#include "pch.h"
#include "Parser.h"

using namespace std;
using namespace vxe::med;

void rotate32(uint32_t& value) {
	unsigned int high, low;
	high = value << 16;
	low = value >> 16;
	value = high | low;
}

template<typename T>
void DICOMParser::ReadBinary(DataElement& DE) {
	// NOTE (Bounds checking): VALUE_FIELD might contain trailing data
	if (DE.vm) {
		if( (sizeof(T) * DE.vm) > DE.length){ 
			printf("\nBounds checking error!");
		}
	} else {
		DE.vm = DE.length / sizeof(T);
	}

	std::vector<T> container;
	container.resize(DE.vm);
	std::memcpy(container.data(), _filebuffer.data() + _pos, DE.length);
	DE.data.emplace<std::vector<T>>(container);
}

void DICOMParser::ReadString(DataElement& DE) {
	std::string inputstr(DE.length, '\0');
	std::string delimiter = "\\";
	std::vector<std::string> container;
	size_t count = 0;

	if (DE.vm) { container.reserve(DE.vm); }
	std::memcpy(inputstr.data(), _filebuffer.data() + _pos, DE.length);

	// NOTE: std::string::find() returns SIZE_MAX if it fails to find the char
	// NOTE (Bounds checking): at least one of split range or value multiplicity conditions must be satisfied
	for(size_t splitindex = inputstr.find(delimiter);
		splitindex > 0 && splitindex <= DE.length;
		splitindex = inputstr.find(delimiter), inputstr = inputstr.substr(splitindex + 1, inputstr.length()), count++ ) // advance on right bisect
	{
		container.push_back(inputstr.substr(0, splitindex));
	}

	container.push_back(inputstr); count++;// handle the edge case
	DE.vm = count;
	DE.data.emplace<std::vector<std::string>>(container);
}

int32_t StringToInteger(const std::string& str, size_t* idx) {
	return std::stoi(str, idx);
}

long double StringToLongDouble(const std::string& str, size_t* idx) {
	return std::stold(str, idx);
}

template<typename T>
void DICOMParser::ReadNumericString(DataElement& DE) {
	std::string inputstr(DE.length+1, '\0');
	std::string delimiter = "\\";
	std::vector<T> container;
	size_t count = 0;
	if (DE.vm) { container.reserve(DE.vm); }
	std::memcpy(inputstr.data(), _filebuffer.data() + _pos, DE.length);

	std::function<T(const std::string&, size_t*)> convert;
	if (typeid(T) == typeid(int32_t)) {
		convert = static_cast<int32_t(*)(const std::string&, size_t*)>(StringToInteger);
	}
	else if (typeid(T) == typeid(long double)) {
		convert = static_cast<long double(*)(const std::string&, size_t*)>(StringToLongDouble);
	}

	for(size_t idx = 0, i = 0; i < DE.length; idx++, i+=idx, count++) { 
		inputstr = inputstr.substr(idx, DE.length);
		container.push_back(convert(inputstr, &idx));
	}

	DE.vm = count;
	DE.data.emplace<std::vector<T>>(container);
}

template<typename T>
void DICOMParser::ReadStream(DataElement& DE) {
	if (DE.length == UNDEFINED) {
		// TODO:
		// IF the stream is Native Pixel Data or Overlay data, use exisiting attributes about
		// number of frames and height/width to obtain the length
		// ELSE IF the stream is Encapsulated Pixel Data use different use different parsing procedure
		return;
	}
	DE.vm = DE.length / sizeof(T);
	ReadBinary<T>(DE);
	DE.vm = 1;
}

void DICOMParser::ReadSequence(DataElement& DE) {
	std::vector<DataSet> Sequence;
	DataElement end = DE;
	if (DE.length == UNDEFINED) {
		while (end.tag != SequenceDelimitationItem) {
			DataSet currentDataSet;
			end = ReadDataElement(currentDataSet);
			if (end.tag != Item) { break; }
			while(end.tag != ItemDelimitationItem){
				end = ReadDataElement(currentDataSet);
			}
			Sequence.push_back(currentDataSet);
		}
	}
	DE.data.emplace<std::vector<DataSet>>(Sequence);
}

void DICOMParser::ReadValue(DataElement& DE) {
	switch (DE.vr) {
	case AE: case AS: case AT: case CS: case DA: case LO: case LT:
	case ST: case TM: case UI: case DT: case PN: case SH: case UT:
		ReadString(DE); break;
	case DS: ReadNumericString<long double>(DE); break;
	case FD: ReadBinary<double>(DE); break;
	case FL: ReadBinary<float>(DE); break;
	case IS: ReadNumericString<int32_t>(DE); break;
	case OB: ReadStream<std::byte>(DE); break;
	case OW: ReadStream<uint16_t>(DE); break;
	case SL: ReadBinary<int32_t>(DE); break;
	case SQ: ReadSequence(DE); break;
	case SS: ReadBinary<int16_t>(DE); break;
	case UL: ReadBinary<uint32_t>(DE); break;
	case US: ReadBinary<uint16_t>(DE); break;
	}
}

void DICOMParser::LoadNewFile(std::filesystem::path filename) {
	std::ifstream rfile(filename, std::ios::binary | std::ios::ate);
	std::streamsize size = rfile.tellg(); // correct size of get area when accessing binary streams

	rfile.seekg(0, std::ios::beg);

	_filebuffer.resize(size);
	rfile.read(reinterpret_cast<char*>(_filebuffer.data()), size);
}

bool DICOMParser::IsDICOM() {
	const char Magic[] = "DICM";
	_pos = 128; // initial header

	std::string compare(_filebuffer.data()+_pos, _filebuffer.data()+_pos+4);
	_pos += 4;
	if (compare == Magic) {
		cout << "File is DICOM" << endl;
		return true;
	}
	return false;
}


DataElement DICOMParser::ReadDataElement(DataSet& RootSet) {

	// Insert Data Element into a Data Set
	// Construct a Data Element

	DataElement DE;
	// std::shared_ptr<DataElement> DE = RootSet.elements.back();
	DE.tag = 0;
	DE.length = 0;

	// std::copy()
	memcpy(&DE.tag, _filebuffer.data() + _pos, sizeof(char) * 4);
	rotate32(DE.tag);
	_pos += 4;

	DEInfo LUTAttr = _dict->GetDEInfo(DE.tag);
	DE.name = LUTAttr.name;

	switch (DE.tag) {
	case Item:
		_pos += 4;
		// start reading a new data set
		return DE;
	case ItemDelimitationItem:
		_pos += 4;
		// end the current data set
		return DE;
	case SequenceDelimitationItem:
		_pos += 4;
		// end the Sequence
		return DE;
	}

	memcpy(&DE.vr, _filebuffer.data() + _pos, sizeof(char) * 2);
	_pos += 2;

	// OB,OW,SQ,UT VR_TYPES have 2 bytes of reserve (0x0000) following VR_TYPE field,
	// their VALUE_LENGTH field is 4 bytes long and they always have a VM of 1
	switch(DE.vr){
	case OB: case OW: case SQ: case UT:
		DE.vm = 1;
		// skip 2 bytes of reserve
		_pos += 2;
		memcpy(&DE.length, _filebuffer.data() + _pos, sizeof(char) * 4);
		_pos += 4;
		break;
	default:
		// TODO: lookup VM from Registry
		DE.vm = LUTAttr.VM;
		// VALUE_LENGTH field is 2 bytes
		memcpy(&DE.length, _filebuffer.data() + _pos, sizeof(char) * 2);
		_pos += 2;
		break;
	}

	RootSet.elements.push_back(DE);
	ReadValue(RootSet.elements.back());

	if (DE.length != UNDEFINED) {
		_pos += DE.length; // Advance
	}

	return DE;
}

DataSet DICOMParser::ReadMetaInfo() {
	DataSet MetaInfoSet;
	DataElement currentDE;
	while (currentDE.tag != FileMetaInformationGroupLength) {
		currentDE = ReadDataElement(MetaInfoSet);
 	}
	uint32_t endpos = _pos + MetaInfoSet.elements.back().FetchValue<uint32_t>();
	while (_pos < endpos) {
		currentDE = ReadDataElement(MetaInfoSet);
 	}
	return MetaInfoSet;
}

DataSet DICOMParser::ReadMainDataSet(TransferSyntax TS) {
	DataSet MainDataSet;
	// Set correct Transfer Syntax
	while (_pos < _filebuffer.size()) {
		ReadDataElement(MainDataSet);
	}
	return MainDataSet;
}
