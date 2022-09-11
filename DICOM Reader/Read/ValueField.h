#pragma once
#include <variant>

#include "ValueRepresentation.h"
#include "DataSet.h"

namespace vxe::med {
	using ValueField = std::variant<
		std::monostate,
		std::vector<std::string>,
		std::vector<uint16_t>,
		std::vector<uint32_t>,
		std::vector<uint64_t>,
		std::vector<int16_t>,
		std::vector<int32_t>,
		std::vector<double>,
		std::vector<float>,
		std::vector<long double>,
		std::vector<std::byte>,
		std::vector<DataSet>
		>;
}
