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
		uint8_t mipmap;
		uint8_t elsize;
	};
	#pragma pack(pop)

	struct BitmapImage {
		BitmapHeader header;
		std::vector<uint8_t> pixeldata;
	};

	// TODO: Use file hash to uniquely identify a file?
	struct CacheEntry {
		std::wstring UID;
		std::wstring type;
		std::wstring Filename;
	};

	class FileCache {
	public:
		FileCache() : _cachefilepath(std::filesystem::current_path().append("DICOMVolumeDB")){}
		FileCache(std::filesystem::path cachepath) : _cachefilepath(cachepath){}
		void ReadCache();
		void AddNewEntry(const CacheEntry& entry);
		std::wstring Query(std::wstring UID, std::wstring type);
		std::filesystem::path GetCachePath() { return _cachefilepath; }

		~FileCache() {
			_cachefile.close();
		}

	protected:
		std::filesystem::path _cachefilepath;
		std::wfstream _cachefile;
		std::vector<CacheEntry> _filecache;
	};
}

namespace vxe::med {

	// Used for gathering volumes and interpolating slices
	class DICOMConverter {
	public:
		DICOMConverter(){}
		DICOMConverter(FileSet& FS) : _MainFileSet(FS) { }
		DICOMConverter(DICOMReader reader) : DICOMConverter(reader.MainFileSet) {}
		DICOMConverter(std::wstring dirname) : DICOMConverter(DICOMReader(dirname)) {}

		virtual void Convert() = 0;
		void WriteBMP(DICOMInstance&, std::filesystem::path filename);
		BitmapImage GetBMP(DICOMInstance& instance);
		void WriteTexture3D(Array3D<uint8_t>& Volume, fs::path filename);
		void WriteTexture3DWithHeader(Array3D<uint8_t>& Volume, std::filesystem::path filename);
		template<typename T>
		void WriteVanityVertex(	const T* vertices, const uint16_t* index,
								const uint32_t& vcount, const uint32_t& icount, std::filesystem::path filename);
		void GatherVolumes();
		void InitializeVolumes();
		void LoadVolumes();

	public:
		std::vector<DICOMVolume> volumeset;
	protected:
		FileSet _MainFileSet;
	};

	template<typename T>
	void DICOMConverter::WriteVanityVertex(	const T* vertices,
											const uint16_t* indices,
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
		fwrite(vertices, sizeof(T), vcount, imgfp);
		fwrite(indices, sizeof(uint16_t), icount, imgfp);
		fclose(imgfp);
	}

}
