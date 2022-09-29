#pragma once

#include "..\pch.h"
#include "Registry/DataElementTags.h"
#include "ValueField.h"

namespace vxe::med {
	struct ge { uint16_t group, element; };

	class DataElement {
	public:
		DataElement();
		DataElement(std::string name, tag_t tag, uint16_t vr, size_t vm, size_t length);

		template<typename T>
		std::vector<T> FetchContainer() const;
		template<typename T>
		T FetchValue(size_t n = 0) const;

		const ValueField GetValue() const;
		bool IsPresent() const {
			return tag ? true : false;
		}

		std::string name;
		tag_t tag;
		uint16_t vr{0};
		size_t vm{0};
		size_t length{0};
		ValueField data;
	}; 


	template<typename T>
	std::vector<T> DataElement::FetchContainer() const {
		return std::get<std::vector<T>>(data);
	}

	template<typename T>
	T DataElement::FetchValue(size_t n) const {
		if(vm != 0)
			return std::get<std::vector<T>>(data)[n];

		return T{0};
	}
}
