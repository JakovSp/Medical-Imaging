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

	struct BitmapImage {
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
}

namespace vxe::med {

	class DICOMConverter {
	public:
		DICOMConverter(std::wstring dirname, FileSet& FS) : _outputdirpath(dirname), _MainFileSet(FS) {
			if (!std::filesystem::exists(_outputdirpath)) {
				_outputdirpath = std::filesystem::current_path();
			}
			DetectVolumes();
		}
		DICOMConverter(DICOMReader reader) : _MainFileSet(reader.MainFileSet) {
			DetectVolumes();
		}
		DICOMConverter(std::wstring dirname) : _MainFileSet(DICOMReader(dirname).MainFileSet) {
			DetectVolumes();
		}

		virtual void Write();
		void WriteBMP(DICOMInstance& instance, std::filesystem::path filename);
		void WriteTexture(Cloud3D<uint8_t>& Volume, std::filesystem::path filename);
		template<typename T>
		void WriteVanityVertex(	const std::vector<T>& vertices, const std::vector<uint16_t>& index,
								const uint32_t& vcount, const uint32_t& icount, std::filesystem::path filename);

	public:
		std::vector<DICOMVolume> volumes;
	private:
		void DetectVolumes();
		void AddVolume(DICOMSeries& NewSeries);
		std::filesystem::path _outputdirpath;
		FileSet _MainFileSet;
	};
}
