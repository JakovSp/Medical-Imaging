#include <pch.h>
#include "Converter.h"

using namespace vxe::med;
using namespace vxe::utl;
using namespace vxe::geo;
using namespace std;
namespace fs = std::filesystem;

// NOTE: Handles Native Image
BitmapImage DICOMConverter::GetBMP(DICOMInstance& instance) {
	BitmapImage image;
	BitmapHeader& header = image.header;

	header.biIH.biWidth = instance[Columns].FetchValue<uint16_t>();
	header.biIH.biHeight = instance[Rows].FetchValue<uint16_t>();
	header.biIH.biPlanes = instance[NumberOfFrames].FetchValue<uint16_t>();
	if (!header.biIH.biPlanes) { header.biIH.biPlanes = 1; }

	header.biIH.biBitCount = 24;
	header.biIH.biCompression = 0;
	header.biIH.biSizeImage = header.biIH.biWidth * header.biIH.biHeight * header.biIH.biBitCount >> 3;
	header.biFH.bfOffBits = sizeof(BitmapHeader);
	header.biFH.bfSize = sizeof(BitmapHeader) + header.biIH.biSizeImage;

	// NOTE: When LUT VOI (Windowing attributes) is not present, they are zero
	long double windowcenter = instance[WindowCenter].FetchValue<long double>();
	long double windowwidth = instance[WindowWidth].FetchValue<long double>();
	DataElement PixelDataDE = instance[PixelData];

	vector<uint8_t> realpixeldata;
	if (PixelDataDE.vr == OW) {
		size_t step = 2;
		const vector<uint16_t>& pixeldata = PixelDataDE.FetchContainer<uint16_t>();

		realpixeldata.reserve(pixeldata.size());
		for (size_t i = 0; i < pixeldata.size(); i++) {
			realpixeldata.push_back(LinearWindowClip(windowcenter, windowwidth, pixeldata[i]));
			realpixeldata.push_back(realpixeldata.back());
			realpixeldata.push_back(realpixeldata.back());
		}
	}

	image.pixeldata = realpixeldata;
	return image;
}

void DICOMConverter::WriteBMP(DICOMInstance& instance, fs::path filename) {
	auto image = GetBMP(instance);
	FILE* imgfp;
	fopen_s(&imgfp, filename.string().c_str(), "wb+");
	if (!imgfp) {
		printf("\nCannot open file for writing a BMP file!");
		return;
	}

	fwrite(&image.header, sizeof(BitmapHeader), 1, imgfp);
	fwrite(image.pixeldata.data(), image.header.biIH.biSizeImage, 1, imgfp);
	fclose(imgfp);
}

void DICOMConverter::WriteTexture3D(Array3D<uint8_t>& Volume, fs::path filename) {
	FILE* imgfp;

	fopen_s(&imgfp, filename.string().c_str(), "wb+");
	if (!imgfp) {
		printf("\nCannot open file for writing a 3D texture!");
		return;
	}

	fwrite(Volume.Points(), sizeof(uint8_t), Volume.Height() * Volume.Width() * Volume.Depth(), imgfp);
	fclose(imgfp);
}

void DICOMConverter::WriteTexture3DWithHeader(Array3D<uint8_t>& Volume, fs::path filename) {
	TextureDescription desc{0};
	FILE* imgfp;

	fopen_s(&imgfp, filename.string().c_str(), "wb+");
	if (!imgfp) {
		printf("\nCannot open file for writing a 3D texture!");
		return;
	}

	desc.depth = Volume.Depth();
	desc.width = Volume.Width();
	desc.height = Volume.Height();
	desc.mipmap = 1;
	desc.elsize = 1; // sizeof(uint8_t)

	fwrite(&desc, sizeof(desc), 1, imgfp);
	fwrite(Volume.Points(), desc.elsize, Volume.Height() * Volume.Width() * Volume.Depth(), imgfp);
	fclose(imgfp);
}


// NOTE: Multiple Series within a Study may share the same Frame of Reference
// NOTE: All images in a Series that share the same Frame of Reference shall be
// spatially related to each other
void DICOMConverter::GatherVolumes() {
	// For each DICOM Study read all of the Frame of References for each series
	for (DICOMPatient& patient : _MainFileSet.Root.patients) {
		for (DICOMStudy& study : patient.studies) {
			for (DICOMSeries& series : study.series) {
				const string& FORUID = series[0][FrameOfReferenceUID].FetchValue<string>();
				if (series.IsHomogeneous(FORUID)) {
					for (size_t id = 0; id < volumeset.size(); id++) {
						if (FORUID == volumeset[id].FORUID) {
							volumeset[id].series.push_back(series);
							return;
						}
					}
					volumeset.push_back(FORUID);
					volumeset.back().series.push_back(series);
				}
			}
		}
	}
	// TODO: Categorize NewSeries by their orientation
}

void DICOMConverter::InitializeVolumes() {
	for (DICOMVolume& volume : volumeset) {
		volume.InitializeVolume();
	}
}

void DICOMConverter::LoadVolumes() {
	for (DICOMVolume& volume : volumeset) {
		volume.LoadVolume();
	}
}

void FileCache::ReadCache() {
	wstring line;
	_cachefile.open(_cachefilepath, ios::app|ios::in|ios::out);

	// _cachefile.seekg(ios::beg);
	while(getline(_cachefile, line, L'\n')) {
		wstringstream issentry(line);

		wstring UID, type, Filename;
		getline(issentry, UID, L',');
		getline(issentry, type, L',');
		getline(issentry, Filename, L'\n');
		if (UID.empty() || Filename.empty())
			continue;

		_filecache.push_back({UID, type, Filename});
	}
	// TODO: Check if a file exists and if it doesn't remove from cache
	_cachefile.close();
}

void FileCache::AddNewEntry(const CacheEntry& entry) {
	_filecache.push_back(entry);
	_cachefile.open(_cachefilepath, ios::app|ios::in|ios::out);
	_cachefile << entry.UID << L"," << entry.type << L"," << entry.Filename << '\n';

	_cachefile.close();
}

wstring FileCache::Query(wstring UID, wstring type) {
	for (CacheEntry& entry : _filecache) {
		if (entry.UID == UID && entry.type == type) {
			return entry.Filename;
		}
	}
	return L"";
}
