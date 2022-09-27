#include "Volume.h"

using namespace std;
using namespace vxe::med;
using namespace vxe::utl;


void DICOMVolume::SetMajorSeries() {
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
// NOTE: Can be ignored
void DICOMVolume::SetMajorOrient() {
	size_t majororient = 0;
	size_t majororientx = 0;
	size_t majororienty = 0;
	vector<long double> orient = series[_majorseries][0][ImageOrientationPatient].FetchContainer<long double>();
	// find biggest x cosine (indices 0,1,2)
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

	// find biggest y cosine (indices 3,4,5)
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

	// select which of x, y or z components is the most dominant
	switch (majororientx + majororienty) {
	case 1: _majororient = 2; break;
	case 2: _majororient = 1; break;
	case 3: _majororient = 0; break;
	}
}

void DICOMVolume::LoadVolume() {
	if (_sampledvolume.Points()) {
		// Some data is loaded
		return;
	}

	SetMajorSeries();
	SetMajorOrient();

	auto orderedslices = OrderSlices();

	_sampledvolume.Allocate(_width, _height, _depth);
	size_t i = 0;
	for (auto it = orderedslices.begin(); it != orderedslices.end(); it++, i++) {
		string sliceUID = it._Ptr->_Myval.second;
		vector<uint16_t> pixeldata = series[_majorseries][sliceUID][PixelData].FetchContainer<uint16_t>();
		_sampledvolume[i].Copy(pixeldata.data());
	}
}

list<pair<long double, string>> DICOMVolume::OrderSlices() {
	// If multiple series are involved:
	// Determine if the orientation of two series are
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
		// TODO: Copy OB Pixel Data
	}
}

Array3D<uint8_t> DICOMVolume::GenerateIsoSamples(int lowerbound, int upperbound) {
	long double wc = series[0][0][WindowCenter].FetchValue<long double>();
	long double ww = series[0][0][WindowWidth].FetchValue<long double>();
	Array3D<uint8_t> IsoTexture(_width, _height, _depth);

	for (size_t i = 0; i < _depth; i++) {
		for (size_t j = 0; j < _width; j++) {
			for (size_t k = 0; k < _height; k++) {
				if (criterion<uint16_t>(_sampledvolume[i][j][k], lowerbound, upperbound)) {
					IsoTexture[i][j][k] = LinearWindowClip(wc, ww, _sampledvolume[i][j][k]);
				}
				else {
					IsoTexture[i][j][k] = 0;
				}
			}
		}
	}

	return IsoTexture;
}

Array3D<uint8_t> DICOMVolume::GenerateIsoSamples(Matter matter) {
	return GenerateIsoSamples(HounsfieldScale[matter].lower, HounsfieldScale[matter].upper);
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
	size_t depth = _sampledvolume.Depth();
	size_t width = _sampledvolume.Width();
	size_t height = _sampledvolume.Height();
	vector<vert3> vertices;

	// TODO: Select the correct series, the one that was used in construction of Array3D
	vector<long double> spacing = GetPixelSpacing(series[0][0].MainDataSet);

	for (size_t i = 0; i < depth; i++) {
		for (size_t j = 0; j < width; j++) {
			for (size_t k = 0; k < height; k++) {
				if (criterion<uint16_t>(_sampledvolume[i][j][k], lowerbound, upperbound)) {
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

Array3D<uint8_t> DICOMVolume::GenerateWindowedSamples() {
	Array3D<uint8_t> Windowed(_width, _height, _depth);
	DataElement VOI = series[0][0][WindowCenter];
	//long double wc = series[0][0][WindowCenter].FetchValue<long double>();
	//long double ww = series[0][0][WindowWidth].FetchValue<long double>();
	for (size_t i = 0; i < _depth; i++) {
		for (size_t j = 0; j < _width; j++) {
			for (size_t k = 0; k < _height; k++) {
				Windowed[i][j][k] = (uint8_t)(_sampledvolume[i][j][k]/2);
			}
		}
	}
	return Windowed;
}
