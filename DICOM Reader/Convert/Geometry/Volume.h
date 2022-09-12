/*
Volume.h

	Volumetric Data with DICOM information
	One DICOMVolume can span multiple series
*/
#pragma once
#include <string>
#include <Read/FileSet.h>
#include "../Hounsfield.h"

#include "Geometry.h"
#include "Cloud3D.h"
#include "../Filter.h"

using namespace vxe::utl;
using namespace vxe::geo;

namespace vxe::med {
	// NOTE: Additional meta information used for the purpose of combining multiple series into one volume
	//		 through interpolation
	struct DICOMSlice {
		DICOMInstance& instance;
		long double Location;
		long double Orientation[3][2];
		long double Position[3];
	};

	class DICOMVolume {
	public:
		DICOMVolume(std::string FOR) : ID(FOR), _majorseries(0), _majororient(0),
			_width(0), _height(0), _depth(0)
		{}

		std::list<std::pair<long double, std::string>> OrderSlices();
		void SetMajorAxis();
		void SetMajorOrient();

		void LoadVolume();

		Cloud3D<uint8_t> GenerateWindowedVolume();
		Cloud3D<uint8_t> GenerateIsoTexture3D(int lowerbound, int upperbound);
		Cloud3D<uint8_t> GenerateIsoTexture3D(Matter matter);
		std::vector<long double> GetPixelSpacing(DataSet DS);
		std::vector<vert3> GenerateIsoPointCloud(Matter matter);
		std::vector<vert3> GenerateIsoPointCloud(int lowerbound, int upperbound);

	public:
		std::string ID;
		std::vector<DICOMSeries> series;
		Cloud3D<uint16_t> Volume;
	private:
		size_t _majorseries;
		size_t _majororient;
		size_t _width, _height, _depth;
	};

}
