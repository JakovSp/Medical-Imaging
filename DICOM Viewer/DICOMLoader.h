/*
Convert.h
	
	Loads volume & geometry assets from DICOM File Set
*/
#pragma once

#include "pch.h"
#include "Textures.h"
#include <DICOM Reader/Read/Reader.h>
#include <DICOM Reader/Convert/Converter.h>

#include <Engine\Models\Mesh.h>
#include <Engine\Third Party\DirectX Tool Kit\VertexTypes.h>

// TODO: figure out what to do with encoding
#include <locale>
#include <codecvt>
#include <string>

// TODO: Use file picker from Windows API
namespace vxe::med {
	template<typename T, typename U>
	struct Geometry{
		std::vector<T> vertices;
		std::vector<U> indices;
	};

	class DICOMLoader : public DICOMConverter {
	public:
		DICOMLoader(std::wstring dirname);
		std::wstring DICOMLoader::GenerateDefaultFilename(std::wstring filetype);

		std::shared_ptr<Mesh<DirectX::VertexPosition, uint16_t>>
		LoadWireframeMesh(Matter matter, std::vector<concurrency::task<void>>& tasks, std::shared_ptr<VanityCore>& vanitycore);
		std::shared_ptr<Mesh<DirectX::VertexPosition, uint16_t>>
		LoadPointCloud(	Matter matter, std::vector<concurrency::task<void>>& tasks, std::shared_ptr<VanityCore>& vanitycore);
		SceneTexture<Texture3D>
		LoadTexture3D( Matter matter, std::vector<concurrency::task<void>>& tasks, std::shared_ptr<VanityCore>& vanitycore);

		virtual void Convert() {

		}



	private:
		std::filesystem::path _localfolderpath;
		std::filesystem::path _installfolderpath;
		bool _caching = true;
		FileCache _cache;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> _utfconverter;
	};
}
