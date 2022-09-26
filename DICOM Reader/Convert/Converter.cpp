#include "Converter.h"

using namespace vxe::med;
using namespace vxe::utl;
using namespace vxe::geo;

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

	std::vector<uint8_t> realpixeldata;
	if (PixelDataDE.vr == OW) {
		size_t step = 2;
		const std::vector<uint16_t>& pixeldata = PixelDataDE.FetchContainer<uint16_t>();

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

void DICOMConverter::WriteBMP(DICOMInstance& instance, std::filesystem::path filename) {
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

void DICOMConverter::WriteTexture(Array3D<uint8_t>& Volume, std::filesystem::path filename) {
	TextureDescription desc;
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

// NOTE: Multiple Series within a Study may share the same Frame of Reference
// NOTE: All images in a Series that share the same Frame of Reference shall be
// spatially related to each other
void DICOMConverter::GatherVolumes() {
	// For each DICOM Study read all of the Frame of References for each series
	for (DICOMPatient& patient : _MainFileSet.Root.patients) {
		for (DICOMStudy& study : patient.studies) {
			for (DICOMSeries& series : study.series) {
				const std::string& FORUID = series[0][FrameOfReferenceUID].FetchValue<std::string>();
				if (series.IsHomogeneous(FORUID)) {
					for (size_t id = 0; id < volumes.size(); id++) {
						if (FORUID == volumes[id].FORUID) {
							volumes[id].series.push_back(series);
							return;
						}
					}
					volumes.push_back(FORUID);
					volumes.back().series.push_back(series);
				}
			}
		}
	}
	// TODO: Categorize NewSeries by their orientation
}
