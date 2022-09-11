#pragma once
#include <cstddef>
#include <cstdint>
#include <limits>

namespace vxe::utl {
	std::byte WindowClip(long double c, long double w, uint16_t x);

	inline bool criterion(float value, float threshold) {
		return value >= threshold ? true : false;
	}

	inline bool criterion(float value, float lower, float upper) {
		return value >= lower ? (value <= upper ? true : false) : false;
	}

	template<typename T>
	bool criterion(T value, T lower, T upper) {
		return (value < lower ? false : (value > upper ? false : true));
	}

	template<typename T>
	T clipvalue(T value, T cuttoff) {
		return (value < cuttoff ? std::numeric_limits<T>::min() : std::numeric_limits<T>::max());
	}
	template<typename T>
	T clipvalue(T value, T a, T b) {
		return (value < a ? std::numeric_limits<T>::min() : (value > b ? std::numeric_limits<T>::max() : value));
	}
}
