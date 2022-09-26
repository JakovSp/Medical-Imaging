#pragma once
#include <string>
#include <filesystem>
#include<iostream>
#include<fstream>

#include <Read/Reader.h>
#include <Read/FileSet.h>
#include "Geometry/Volume.h"
#include "Geometry/MarchingCubes.h"
#include "Hounsfield.h"

namespace vxe::utl {
// correctly pack structures, do not allow any padding, intended for writing
	#pragma pack(push, 1)
		struct BITMAPFILEHEADER {
			uint16_t bfType = 19778;
			uint32_t bfSize = 0;
			uint32_t bfReserved = 0;
			uint32_t bfOffBits = 1708;
		};

		struct BITMAPINFOHEADER {
			uint32_t biSize = sizeof(struct BITMAPINFOHEADER);
			uint32_t biWidth;
			uint32_t biHeight;
			uint16_t biPlanes;
			uint16_t biBitCount;
			uint32_t biCompression;
			uint32_t biSizeImage;
			uint32_t biXPelsPerMeter = 0;
			uint32_t biYPelsPerMeter = 0;
			uint32_t biClrUsed = 0;
			uint32_t biClrImportant = 0;
		};

	struct BitmapHeader {
		BITMAPFILEHEADER biFH;
		BITMAPINFOHEADER biIH;
	};

	struct TextureDescription {
		uint32_t width;
		uint32_t height;
		uint32_t depth;
		uint16_t mipmap;
		uint16_t elsize;
	};
	#pragma pack(pop)

	struct BitmapImage {
		BitmapHeader header;
		std::vector<uint8_t> pixeldata;
	};
}

namespace vxe::med {

	class DICOMConverter {
	public:
		DICOMConverter(FileSet& FS) :_MainFileSet(FS) {
			GatherVolumes();
		}
		DICOMConverter(DICOMReader reader) : DICOMConverter(reader.MainFileSet) {}
		DICOMConverter(std::wstring dirname) : DICOMConverter(DICOMReader(dirname)) {}

		virtual void Convert() = 0;
		void WriteBMP(DICOMInstance&, std::filesystem::path filename);
		BitmapImage GetBMP(DICOMInstance& instance);
		void WriteTexture(Array3D<uint8_t>& Volume, std::filesystem::path filename);
		template<typename T>
		void WriteVanityVertex(	const std::vector<T>& vertices, const std::vector<uint16_t>& index,
								const uint32_t& vcount, const uint32_t& icount, std::filesystem::path filename);
		void GetVolume(std::string studyuid, std::string foruid) {
		}

	public:
		std::vector<DICOMVolume> volumes;
	private:
		void GatherVolumes();
		FileSet& _MainFileSet;
	};
}
