#pragma once
#include "Cloud3D.h"

using namespace vxe::utl;

namespace vxe::geo {
	struct vert3 {
		float x, y, z;
		vert3() :x(0), y(0), z(0) {}
		vert3(float vx, float vy, float vz) :x(vx), y(vy), z(vz) {}
	};

	struct GridCell {
		vert3 p[8];
		float val[8];
		GridCell(Array3D<uint16_t>& Volume, size_t x, size_t y, size_t z) {
			val[0] = Volume[z + 1][y + 1][x + 0];
			val[1] = Volume[z + 1][y + 1][x + 1];
			val[2] = Volume[z + 0][y + 1][x + 1];
			val[3] = Volume[z + 0][y + 1][x + 0];
			val[4] = Volume[z + 1][y + 0][x + 0];
			val[5] = Volume[z + 1][y + 0][x + 1];
			val[6] = Volume[z + 0][y + 0][x + 1];
			val[7] = Volume[z + 0][y + 0][x + 0];
		}

		void CalculatePosition(Array3D<uint16_t>& Volume, size_t x, size_t y, size_t z) {
			size_t width = Volume.Width();
			size_t height = Volume.Height();
			size_t depth = Volume.Depth();
			p[0] = vert3((double)(x + 0) / width, (double)(y + 1) / height, (double)(z + 1) / depth);
			p[1] = vert3((double)(x + 1) / width, (double)(y + 1) / height, (double)(z + 1) / depth);
			p[2] = vert3((double)(x + 1) / width, (double)(y + 1) / height, (double)(z + 0) / depth);
			p[3] = vert3((double)(x + 0) / width, (double)(y + 1) / height, (double)(z + 0) / depth);
			p[4] = vert3((double)(x + 0) / width, (double)(y + 0) / height, (double)(z + 1) / depth);
			p[5] = vert3((double)(x + 1) / width, (double)(y + 0) / height, (double)(z + 1) / depth);
			p[6] = vert3((double)(x + 1) / width, (double)(y + 0) / height, (double)(z + 0) / depth);
			p[7] = vert3((double)(x + 0) / width, (double)(y + 0) / height, (double)(z + 0) / depth);
		}
	};

	struct tri {
		vert3 p1, p2, p3;
		tri() {}
		tri(vert3 tp1, vert3 tp2, vert3 tp3) : p1(tp1), p2(tp2), p3(tp3) {}
	};
}
