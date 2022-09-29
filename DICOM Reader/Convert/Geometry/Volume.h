/*
_sampledvolume.h

	Volumetric Data with DICOM information
	One DICOMVolume can span multiple series
*/
#pragma once
#include "pch.h"
#include <Read/FileSet.h>
#include "../Hounsfield.h"

#include "Geometry.h"
#include "Cloud3D.h"
#include "..\Filter.h"

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
		DICOMVolume(std::string FOR) : FORUID(FOR), _majorseries(0), _majororient(0),
			_width(0), _height(0), _depth(0), _loaded(false)
		{}

		void InitializeVolume();
		void LoadVolume();
		Array3D<uint16_t> GetSamples() { return _sampledvolume; }
		std::vector<long double> GetPixelSpacing(DataSet DS);

		Array3D<uint8_t> GenerateWindowedSamples();
		Array3D<uint8_t> GenerateIsoSamples(Matter matter);
		std::vector<vert3> GenerateIsoPointCloud(Matter matter);

	private:
		void SetMajorSeries();
		void SetMajorOrient();
		std::list<std::pair<long double, std::string>> OrderSlices();
		std::vector<vert3> GenerateIsoPointCloud(float lowerbound, float upperbound);
		Array3D<uint8_t> GenerateIsoSamples(float lowerbound, float upperbound);

	public:
		// NOTE: Combination of Frame of Reference UID and Study Instance UID is
		// sufficient to uniquely identify a DICOM Volume
		std::string FORUID;
		std::string StudyUID;
		std::vector<DICOMSeries> series;
	private:
		Array3D<uint16_t> _sampledvolume;
		size_t _majorseries;
		size_t _majororient;
		size_t _width, _height, _depth;
		bool _loaded;
	};

}
