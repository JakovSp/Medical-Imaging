#include "Volume.h"

using namespace std;
using namespace vxe::med;
using namespace vxe::utl;


void DICOMVolume::SetMajorAxis() {
	// Criteria: consider the one with the most number of samples w/r to space spanned(density)?
	// Could be implied by Slice Thickness or Spacing Between Slices attributes.
	long double minspacing = numeric_limits<long double>::max();
	for (size_t i = 0; i < series.size(); i++) {
		long double spacing = series[i][0][SpacingBetweenSlices].FetchValue<long double>();
		if (minspacing > spacing) {
			_majorseries = i;
			minspacing = spacing;
		}
	}

	_width = series[_majorseries][0][Columns].FetchValue<uint16_t>();
	_height = series[_majorseries][0][Rows].FetchValue<uint16_t>();
	_depth = series[_majorseries].instances.size();
	
}

// Approximate the direction in R^3, by just choosing the axis with the biggest cosine component
// Less precision is necessary when ordering slices in space
void DICOMVolume::SetMajorOrient() {
	size_t majororient = 0;
	size_t majororientx = 0;
	size_t majororienty = 0;
	vector<long double> orient = series[_majorseries][0][ImageOrientationPatient].FetchContainer<long double>();
	// find biggest x cosine
	if (orient[0] < orient[1]) {
		if (orient[1] < orient[2]) {
			majororientx = 2;
		} else {
			majororientx = 1;
		}
	}
	else {
		if (orient[0] < orient[2]) {
			majororientx = 2;
		} else {
			majororientx = 0;
		}

	}
	// find biggest y cosine
	if (orient[3] < orient[4]) {
		if (orient[4] < orient[5]) {
			majororienty = 2;
		} else {
			majororienty = 1;
		}
	}
	else {
		if (orient[3] < orient[5]) {
			majororienty = 2;
		} else {
			majororienty = 0;
		}

	}
	switch (majororientx + majororienty) {
	case 1: _majororient = 2; break;
	case 2: _majororient = 1; break;
	case 3: _majororient = 0; break;
	}
}

void DICOMVolume::LoadVolume() {
	SetMajorAxis();
	SetMajorOrient();

	auto orderedslices = OrderSlices();

	Volume.Allocate(_width, _height, _depth);
	size_t i = 0;
	for (auto it = orderedslices.begin(); it != orderedslices.end(); it++, i++) {
		string sliceUID = it._Ptr->_Myval.second;
		vector<uint16_t> pixeldata = series[_majorseries][sliceUID][PixelData].FetchContainer<uint16_t>();
		Volume[i].Copy(pixeldata.data());
	}
}

list<pair<long double, string>> DICOMVolume::OrderSlices() {
	// If multiple series are involved:
	// Determine if the _orientation of two series are
	// significantly different from each other to warrant an interpolation
	// If they are: Choice of sampling axis (which series is major)?

	DataElement SeriesPixelDataDE = series[_majorseries][0][PixelData];
	if (SeriesPixelDataDE.vr == OW) {
		// Copy OW Pixel Data:

		list<pair<long double, string>> orderedslices;
		// Slice Location attribute is the universal way to order slices in space, instead of instance number
		// It is a Type 3 attribute (optional), so it is necessary to use Image Position(Patient) and
		// Image Orientation(Patient) to calculate the slice location explicitly


		for (size_t i = 0; i < _depth; i++) {
			bool inserted = false;
			long double slicelocation = 0;
			if (series[_majorseries][i][SliceLocation].IsPresent()){
				slicelocation = series[_majorseries][i][SliceLocation].FetchValue<long double>();
			}
			else {
				slicelocation = series[_majorseries][i][ImagePositionPatient].FetchValue<long double>(_majororient);
			}
			for ( auto it = orderedslices.begin(); it != orderedslices.end(); ++it) {
				if (slicelocation < it._Ptr->_Myval.first) {
					orderedslices.insert(it, make_pair(slicelocation, series[_majorseries][i][SOPInstanceUID].FetchValue<string>()));
					inserted = true;
					break;
				}
			}
			if (!inserted) {
				orderedslices.push_back(make_pair(slicelocation, series[_majorseries][i][SOPInstanceUID].FetchValue<string>()));
			}
		}

		return orderedslices;
	} else {
		// Copy OB Pixel Data:
	}
}

Cloud3D<uint8_t> DICOMVolume::GenerateIsoTexture3D(int lowerbound, int upperbound) {
	long double wc = series[0][0][WindowCenter].FetchValue<long double>();
	long double ww = series[0][0][WindowWidth].FetchValue<long double>();
	Cloud3D<uint8_t> IsoTexture(_width, _height, _depth);

	for (size_t i = 0; i < _depth; i++) {
		for (size_t j = 0; j < _width; j++) {
			for (size_t k = 0; k < _height; k++) {
				if (criterion<uint16_t>(Volume[i][j][k], lowerbound, upperbound)) {
					IsoTexture[i][j][k] = LinearWindowClip(wc, ww, Volume[i][j][k]);
				}
				else {
					IsoTexture[i][j][k] = 0;
				}
			}
		}
	}

//	uint8_t* texturedata = IsoTexture.Points();
//	auto texturecontainer = vector<uint8_t>(texturedata, texturedata + depth * width * height);
	return IsoTexture;
}

Cloud3D<uint8_t> DICOMVolume::GenerateIsoTexture3D(Matter matter) {
	return GenerateIsoTexture3D(HounsfieldScale[matter].lower, HounsfieldScale[matter].upper);
}

vector<long double> DICOMVolume::GetPixelSpacing(DataSet DS) {
	vector<long double> spacing = DS[PixelSpacing].FetchContainer<long double>();
	spacing.push_back(1.0f); // default
	long double& dz = spacing[2];

	// Best option is to use SpacingBetweenSlices, second best is SliceThickness
	DataElement SliceSpacingDE = DS[SpacingBetweenSlices];
	if (!SliceSpacingDE.IsPresent()) {
		SliceSpacingDE = DS[SliceThickness];
		if (!SliceSpacingDE.IsPresent()) {
			return spacing;
		}
	}
	dz = SliceSpacingDE.FetchValue<long double>();

	return spacing;
}

vector<vert3> DICOMVolume::GenerateIsoPointCloud(int lowerbound, int upperbound) {
	size_t depth = Volume.Depth();
	size_t width = Volume.Width();
	size_t height = Volume.Height();
	vector<vert3> vertices;

	// TODO: Select the correct series, the one that was used in construction of Cloud3D
	vector<long double> spacing = GetPixelSpacing(series[0][0].MainDataSet);

	for (size_t i = 0; i < depth; i++) {
		for (size_t j = 0; j < width; j++) {
			for (size_t k = 0; k < height; k++) {
				if (criterion<uint16_t>(Volume[i][j][k], lowerbound, upperbound)) {
					vertices.push_back(vert3{	((float)j / width),
												((float)k / height),
												((float)i / depth)});
				}
			}
		}
	}
	return vertices;
}

vector<vert3> DICOMVolume::GenerateIsoPointCloud(Matter matter) {
	return GenerateIsoPointCloud(HounsfieldScale[matter].lower, HounsfieldScale[matter].upper);
}

Cloud3D<uint8_t> DICOMVolume::GenerateWindowedVolume() {
	Cloud3D<uint8_t> Windowed(_width, _height, _depth);
	DataElement VOI = series[0][0][WindowCenter];
	//long double wc = series[0][0][WindowCenter].FetchValue<long double>();
	//long double ww = series[0][0][WindowWidth].FetchValue<long double>();
	for (size_t i = 0; i < _depth; i++) {
		for (size_t j = 0; j < _width; j++) {
			for (size_t k = 0; k < _height; k++) {
				Windowed[i][j][k] = (uint8_t)(Volume[i][j][k]/2);
			}
		}
	}
	return Windowed;
}
