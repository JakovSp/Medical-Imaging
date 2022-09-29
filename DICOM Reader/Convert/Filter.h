#pragma once
#include "pch.h"

namespace vxe::utl {
	uint8_t LinearWindowClip(long double c, long double w, uint16_t x);
	uint8_t SigmoidWindowClip(long double c, long double w, uint16_t x);

	inline bool criterion(const float& value, const float& threshold) {
		return value >= threshold ? true : false;
	}

	inline bool criterion(const float& value, const float& lower, const float& upper) {
		return value >= lower ? (value <= upper ? true : false) : false;
	}

	template<typename T>
	bool criterion(const T& value, const float& lower, const float& upper) {
		return (value < lower ? false : (value > upper ? false : true));
	}

	template<typename T>
	T clipvalue(const T& value, const float& cuttoff) {
		return (value < cuttoff ? std::numeric_limits<T>::min() : std::numeric_limits<T>::max());
	}
	template<typename T>
	T clipvalue(const T& value, const float& a, const float& b) {
		return (value < a ? std::numeric_limits<T>::min() : (value > b ? std::numeric_limits<T>::max() : value));
	}
}
