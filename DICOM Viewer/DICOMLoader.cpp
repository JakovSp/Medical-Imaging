#include "pch.h"

#include "DICOMLoader.h"

#include <DICOM Reader/Convert/Geometry/Geometry.h>
#include <DICOM Reader/Convert/Geometry/Volume.h>
#include <DICOM Reader/Convert/Geometry/MarchingCubes.h>

// TODO: figure out what to do with encoding
#include <locale>
#include <codecvt>
#include <string>

using namespace std;
using namespace vxe;
using namespace DirectX;

using namespace concurrency;
using namespace vxe::med;
using namespace vxe::geo;

std::vector<char> DICOMLoader::LoadPointCloud() {
	volumeset[0].LoadVolume();
	std::vector<vert3> points = volumeset[0].GenerateIsoPointCloud(CorticalBone);
	char* data = (char*)points.data();
	vector<char> deserialized(data, data + points.size()*sizeof(vert3));
	return deserialized;
}

std::vector<char> DICOMLoader::LoadWireframeMesh() {
	volumeset[0].LoadVolume();
	std::vector<tri> Surface = MarchingCubes(volumeset[0].GetSamples(), HounsfieldScale[CorticalBone].lower);
	char* data = (char*)Surface.data();
	vector<char> deserialized(data, data + Surface.size()*sizeof(tri));
	return deserialized;
}

SceneTexture<Texture3D> DICOMLoader::LoadTexture3D(vector<task<void>>& tasks, shared_ptr<VanityCore>& vanitycore) {

	auto device = vanitycore->GetD3DDevice();

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	std::wstring FORUID = converter.from_bytes(volumeset[0].FORUID);
	std::wstring type = converter.from_bytes("CorticalBoneTexture");
	std::filesystem::path textureFilename(QueryCache(FORUID, type));
	if (std::filesystem::exists(textureFilename)) {
		// read file header in bytes
		std::ifstream textureFile(textureFilename, ios::binary | ios::ate);
		TextureDescription texdesc;

		streamsize headersize = sizeof(TextureDescription);
		streamsize imagedatasize = textureFile.tellg() - headersize;

		textureFile.seekg(ios::beg);
		textureFile.read(reinterpret_cast<char*>(&texdesc), headersize);
		std::vector<uint8_t> imagedata(imagedatasize);
		textureFile.read(reinterpret_cast<char*>(imagedata.data()), imagedatasize);

		textureFile.close();

		DXGI_FORMAT format;
		switch(texdesc.elsize){
		case 1: format = DXGI_FORMAT_R8_UNORM; break;
		case 2: format = DXGI_FORMAT_R16_UNORM; break;
		default: format = DXGI_FORMAT_R8_UNORM;
		}


		auto texture3D = make_shared<Texture3D>(device, format, texdesc.width, texdesc.height, texdesc.depth, texdesc.mipmap,
											D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_IMMUTABLE);
		SceneTexture<Texture3D> scenetexture3D(texture3D);

		// load the texture from the rest of the;
		scenetexture3D.Load(tasks, imagedata);
		return scenetexture3D;
	}

	volumeset[0].LoadVolume();
	auto VolumeTexture = volumeset[0].GenerateIsoSamples(CorticalBone);
	size_t depth = VolumeTexture.Depth();
	size_t width = VolumeTexture.Width();
	size_t height = VolumeTexture.Height();

	auto texture3D = make_shared<Texture3D>(device, DXGI_FORMAT_R8_UNORM, width, height, depth, 1,
										D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_IMMUTABLE);
	SceneTexture<Texture3D> scenetexture3D(texture3D);

	uint8_t* texturedata = VolumeTexture.Points();

	if (_caching) {
		std::wstring filepath(Windows::Storage::ApplicationData::Current->LocalFolder->Path->Data());
		filepath.append(L"\\NewVolumeTexture");
		WriteTexture(VolumeTexture, filepath.c_str());
		_filecache.push_back({ FORUID, type, filepath.c_str()});
		WriteCache();
	}

	scenetexture3D.Load(tasks, vector<uint8_t>(texturedata, texturedata + depth * width * height));

	return scenetexture3D;
}
