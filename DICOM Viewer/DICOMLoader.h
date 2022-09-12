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

namespace vxe::med {
	template<typename T, typename U>
	struct Geometry{
		std::vector<T> vertices;
		std::vector<U> indices;
	};

	class DICOMLoader {
	public:
		DICOMLoader(std::wstring dirname) : converter(dirname) {
		}

		std::vector<char> LoadPointCloud();
		std::vector<char> LoadWireframeMesh();
		SceneTexture<Texture3D> LoadTexture3D(	std::vector<concurrency::task<void>>& tasks,
												std::shared_ptr<VanityCore>& vanitycore);

	private:
		DICOMConverter converter;
		std::vector<float> vertices;
	};
}
