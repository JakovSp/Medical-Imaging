#include "Converter.h"

using namespace vxe::med;
using namespace vxe::utl;
using namespace vxe::geo;

void DICOMConverter::WriteBMP(DICOMInstance& instance, std::filesystem::path filename) {
	// Native DICOM Image 
	BitmapImage bmpimg;
	bmpimg.biIH.biWidth = instance.MainDataSet[Columns].FetchValue<uint16_t>();
	bmpimg.biIH.biHeight = instance.MainDataSet[Rows].FetchValue<uint16_t>();
	bmpimg.biIH.biPlanes = instance.MainDataSet[NumberOfFrames].FetchValue<uint16_t>();
	if (!bmpimg.biIH.biPlanes) { bmpimg.biIH.biPlanes = 1; }

	bmpimg.biIH.biBitCount = 24;
	bmpimg.biIH.biCompression = 0;
	bmpimg.biIH.biSizeImage = bmpimg.biIH.biWidth * bmpimg.biIH.biHeight * bmpimg.biIH.biBitCount >> 3;
		// instance.MainDataSet[BitsAllocated].FetchValue<uint16_t>()>>3;
	bmpimg.biFH.bfOffBits = sizeof(BitmapImage);
	bmpimg.biFH.bfSize = sizeof(BitmapImage) + bmpimg.biIH.biSizeImage;

	FILE* imgfp;
	fopen_s(&imgfp, filename.string().c_str(), "wb+");
	if (!imgfp) {
		printf("\nCannot open file for writing a BMP file!");
		return;
	}

	DataElement PixelDataDE = instance.MainDataSet[PixelData];
	long double windowcenter = instance.MainDataSet[WindowCenter].FetchValue<long double>();
	long double windowwidth = instance.MainDataSet[WindowWidth].FetchValue<long double>();

	std::vector<std::byte> realpixeldata;
	if (PixelDataDE.vr == OW) {
		size_t step = 2;
		const std::vector<uint16_t>& pixeldata = PixelDataDE.FetchContainer<uint16_t>();

		realpixeldata.reserve(pixeldata.size());
		for (size_t i = 0; i < pixeldata.size(); i++) {
			realpixeldata.push_back(WindowClip(windowcenter, windowwidth, pixeldata[i]));
			realpixeldata.push_back(realpixeldata.back());
			realpixeldata.push_back(realpixeldata.back());
		}
	}

	fwrite(&bmpimg, sizeof(BitmapImage), 1, imgfp);
	fwrite(realpixeldata.data(), bmpimg.biIH.biSizeImage, 1, imgfp);
	fclose(imgfp);
}

void DICOMConverter::WriteTexture(Cloud3D<uint8_t>& Volume, std::filesystem::path filename) {
	FILE* imgfp;
	fopen_s(&imgfp, filename.string().c_str(), "wb+");
	if (!imgfp) {
		printf("\nCannot open file for writing a 3D texture!");
		return;
	}

	fwrite(Volume.Points(), Volume.Height() * Volume.Width() * Volume.Depth(), 1, imgfp);
	fclose(imgfp);
}

template<typename T>
void DICOMConverter::WriteVanityVertex(	const std::vector<T>& vertices,
										const std::vector<uint16_t>& indices,
										const uint32_t& vcount,
										const uint32_t& icount,
										std::filesystem::path filename) {
	FILE* imgfp;
	fopen_s(&imgfp, filename.string().c_str(), "wb+");
	if (!imgfp) {
		printf("\nCannot open file for writing a Vanity Vertex file!");
		return;
	}

	fwrite(&vcount, sizeof(uint32_t), 1, imgfp);
	fwrite(&icount, sizeof(uint32_t), 1, imgfp);
	fwrite(vertices.data(), sizeof(float), vcount*3, imgfp);
	fwrite(indices.data(), sizeof(uint16_t), icount, imgfp);
	fclose(imgfp);
}

void DICOMConverter::DetectVolumes() {
	// Iterate through FileSet
	for (size_t i = 0; i < _MainFileSet.Root.patients.size(); i++) {
		DICOMPatient patient = _MainFileSet.Root.patients[i];
		for (size_t j = 0; j < patient.studies.size(); j++) {
			// For each DICOM Study read all of the Frame of References for each series
			// All images in a Series that share the same Frame of Reference shall be spatially related to each other
			// Multiple Series within a Study may share the same Frame of Reference
			// The common denominator is DICOM Instance
			DICOMStudy study = patient.studies[j];
			for (size_t k = 0; k < study.series.size(); k++) {
				AddVolume(study.series[k]);
			}
		}
	}
	// TODO: Categorize NewSeries by their orientation
}

void DICOMConverter::AddVolume(DICOMSeries& NewSeries) {
	// Group DICOM Instances by Frame of Reference
	// If the Volume doesn't exist, insert a new one
	const std::string& FORUID = NewSeries.instances[0].MainDataSet[FrameOfReferenceUID].FetchValue<std::string>();
	if (NewSeries.IsHomogeneous(FORUID)) {
		for (size_t id = 0; id < volumes.size(); id++) {
			if (FORUID == volumes[id].ID) {
				volumes[id].series.push_back(NewSeries);
				return;
			}
		}
		volumes.push_back(FORUID);
		volumes.back().series.push_back(NewSeries);
	}
}

void DICOMConverter::Write() {
}
