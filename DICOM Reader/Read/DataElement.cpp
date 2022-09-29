#include "pch.h"
#include "DataElement.h"

using namespace vxe::med;

DataElement::DataElement() : name(""), tag(0), vr(0), vm(0), length(0), data(std::monostate()) {}

const ValueField DataElement::GetValue() const {
	// based on VR set the appropriate ValueField type
	switch (vr) {
	case AE: case AS: case AT: case CS: case DA: case LO: case LT:
	case ST: case TM: case UI: case DT: case PN: case SH: case UT:
		return std::get<std::vector<std::string>>(data);
	case DS: return std::get<std::vector<long double>>(data);
	case FD: return std::get<std::vector<double>>(data);
	case FL: return std::get<std::vector<float>>(data);
	case IS: return std::get<std::vector<int32_t>>(data);
	case OB: return std::get<std::vector<std::byte>>(data);
	case OW: return std::get<std::vector<uint16_t>>(data);
	case SL: return std::get<std::vector<int32_t>>(data);
	case SQ: return std::get<std::vector<DataSet>>(data);
	case SS: return std::get<std::vector<int16_t>>(data);
	case UL: return std::get<std::vector<uint64_t>>(data);
	case US: return std::get<std::vector<uint16_t>>(data);
	}
	return data;
}

DataElement::DataElement(std::string name, tag_t tag, uint16_t vr, size_t vm, size_t length) : 
	name(name), tag(tag_t(tag)), vr(vr), vm(vm), length(length) {
	switch (vr) {
	case AE: case AS: case AT: case CS: case DA: case LO: case LT:
	case ST: case TM: case UI: case DT: case PN: case SH: case UT:
		data.emplace<std::vector<std::string>>(0); break;
	case DS: data.emplace<std::vector<long double>>(0); break;
	case FD: data.emplace<std::vector<double>>(0); break;
	case FL: data.emplace<std::vector<float>>(0); break;
	case IS: data.emplace<std::vector<int32_t>>(0); break;
	case OB: data.emplace<std::vector<std::byte>>(0); break;
	case OW: data.emplace<std::vector<uint16_t>>(0); break;
	case SL: data.emplace<std::vector<int32_t>>(0); break;
	case SQ: data.emplace<std::vector<DataSet>>(0); break;
	case SS: data.emplace<std::vector<int16_t>>(0); break;
	case UL: data.emplace<std::vector<uint64_t>>(0); break;
	case US: data.emplace<std::vector<uint16_t>>(0); break;
	}
}
