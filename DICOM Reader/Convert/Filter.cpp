#include "Filter.h"

namespace vxe::utl {
	uint8_t WindowClip(long double c, long double w, uint16_t x) {
		uint8_t y = 0;
		if (x <= c - 0.5 - (w - 1) / 2) {
			y = 0;
		}
		else if (x > c - 0.5 + (w - 1) / 2) {
			y = 255;
		}
		else {
			y = ((x - (c - 0.5)) / (w - 1) + 0.5) * 255;
		}
		return y;
	}

}
