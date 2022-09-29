#include "pch.h"
#include "MarchingCubes.h"

namespace vxe::geo {
	uint8_t ConfigurationIndex(GridCell cell, const float& cutoff) {
		uint8_t cellvalue = 0;
		// Clamp vertex values from 0 to 1, push to byte by shifting left
		// For each vertex:
		if (criterion( cell.val[0], cutoff)) { cellvalue |= 1 << 0; }
		if (criterion( cell.val[1], cutoff)) { cellvalue |= 1 << 1; }
		if (criterion( cell.val[2], cutoff)) { cellvalue |= 1 << 2; }
		if (criterion( cell.val[3], cutoff)) { cellvalue |= 1 << 3; }
		if (criterion( cell.val[4], cutoff)) { cellvalue |= 1 << 4; }
		if (criterion( cell.val[5], cutoff)) { cellvalue |= 1 << 5; }
		if (criterion( cell.val[6], cutoff)) { cellvalue |= 1 << 6; }
		if (criterion( cell.val[7], cutoff)) { cellvalue |= 1 << 7; }
		return cellvalue;
	}

	uint8_t ConfigurationIndex(GridCell cell, const float& lower, const float& upper) {
		uint8_t cellvalue = 0;
		// Clamp vertex values from 0 to 1, push to byte by shifting left
		// For each vertex:
		if (criterion( cell.val[0], lower, upper)) { cellvalue |= 1 << 0; }
		if (criterion( cell.val[1], lower, upper)) { cellvalue |= 1 << 1; }
		if (criterion( cell.val[2], lower, upper)) { cellvalue |= 1 << 2; }
		if (criterion( cell.val[3], lower, upper)) { cellvalue |= 1 << 3; }
		if (criterion( cell.val[4], lower, upper)) { cellvalue |= 1 << 4; }
		if (criterion( cell.val[5], lower, upper)) { cellvalue |= 1 << 5; }
		if (criterion( cell.val[6], lower, upper)) { cellvalue |= 1 << 6; }
		if (criterion( cell.val[7], lower, upper)) { cellvalue |= 1 << 7; }
		return cellvalue;
	}

	vert3 InterpolateVertex(vert3 p1, vert3 p2, float iso1, float iso2, float isovalue) {
		float d;
		vert3 p;
		// three cases that help close small cracks
		if (std::abs(isovalue - iso1) < 0.00001) { return p1; }
		if (std::abs(isovalue - iso2) < 0.00001) { return p2; }
		if (std::abs(iso2 - iso1) < 0.00001) { return p1; }
		d = (isovalue - iso1) / (iso2 - iso1);
		p.x = p1.x + d * (p2.x - p1.x);
		p.y = p1.y + d * (p2.y - p1.y);
		p.z = p1.z + d * (p2.z - p1.z);
		return p;
	}

	void PolygoniseGridCell(const GridCell& cell, uint8_t lookupindex, std::vector<tri>& Triangles, float isovalue) {
		tri newtri;
		vert3 vertexlist[12];
		if (edgeTable[lookupindex] & 1)
		  vertexlist[0] = InterpolateVertex(cell.p[0],cell.p[1],cell.val[0],cell.val[1], isovalue);
		if (edgeTable[lookupindex] & 2)
		  vertexlist[1] = InterpolateVertex(cell.p[1],cell.p[2],cell.val[1],cell.val[2], isovalue);
		if (edgeTable[lookupindex] & 4)
		  vertexlist[2] = InterpolateVertex(cell.p[2],cell.p[3],cell.val[2],cell.val[3], isovalue);
		if (edgeTable[lookupindex] & 8)
		  vertexlist[3] = InterpolateVertex(cell.p[3],cell.p[0],cell.val[3],cell.val[0], isovalue);
		if (edgeTable[lookupindex] & 16)
		  vertexlist[4] = InterpolateVertex(cell.p[4],cell.p[5],cell.val[4],cell.val[5], isovalue);
		if (edgeTable[lookupindex] & 32)
		  vertexlist[5] = InterpolateVertex(cell.p[5],cell.p[6],cell.val[5],cell.val[6], isovalue);
		if (edgeTable[lookupindex] & 64)
		  vertexlist[6] = InterpolateVertex(cell.p[6],cell.p[7],cell.val[6],cell.val[7], isovalue);
		if (edgeTable[lookupindex] & 128)
		  vertexlist[7] = InterpolateVertex(cell.p[7],cell.p[4],cell.val[7],cell.val[4], isovalue);
		if (edgeTable[lookupindex] & 256)
		  vertexlist[8] = InterpolateVertex(cell.p[0],cell.p[4],cell.val[0],cell.val[4], isovalue);
		if (edgeTable[lookupindex] & 512)
		  vertexlist[9] = InterpolateVertex(cell.p[1],cell.p[5],cell.val[1],cell.val[5], isovalue);
		if (edgeTable[lookupindex] & 1024)
		  vertexlist[10] = InterpolateVertex(cell.p[2],cell.p[6],cell.val[2],cell.val[6], isovalue);
		if (edgeTable[lookupindex] & 2048)
		  vertexlist[11] = InterpolateVertex(cell.p[3],cell.p[7],cell.val[3],cell.val[7], isovalue);

		for (size_t i = 0; triTable[lookupindex][i] != -1; i += 3) {
			Triangles.push_back({});
			Triangles.back().p1 = vertexlist[triTable[lookupindex][i]];
			Triangles.back().p2 = vertexlist[triTable[lookupindex][i+1]];
			Triangles.back().p3 = vertexlist[triTable[lookupindex][i+2]];
		}
	}

	std::vector<tri> MarchingCubes(Array3D<uint16_t> Volume, float isovalue) {
		// Perform a threshold operation
		size_t depth = Volume.Depth();
		size_t width = Volume.Width();
		size_t height = Volume.Height();
		std::vector<tri> Surface;
		Surface.reserve(depth*width*height/8); // Start with generous approximation

		// For each cell:
		for (size_t i = 0; i < depth - 1; i++) {
			for (size_t j = 0; j < width - 1; j++) {
				for (size_t k = 0; k < height - 1; k++) {
					GridCell cell(Volume, j, k, i);
					uint8_t lookupindex = ConfigurationIndex(cell, isovalue);
					// If it is on the iso-surface:
						// If combined value is neither 0 or 255, it's on iso-surface
					if (lookupindex != 0 && lookupindex != 255) {
						cell.CalculatePosition(Volume, j, k, i);
						std::vector<tri> Triangles;
						PolygoniseGridCell(cell, lookupindex, Triangles, isovalue);
						Surface.insert(Surface.end(), Triangles.begin(), Triangles.end());
					}
				}
			}
		}

		// _sampledvolume[i][j][k] = clipvalue<uint16_t>(_sampledvolume[i][j][k], cutoff);
		return Surface;
	}

}
