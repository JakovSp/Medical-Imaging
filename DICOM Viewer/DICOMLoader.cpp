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

DICOMLoader::DICOMLoader(wstring dirname){ 
	_installfolderpath = Windows::ApplicationModel::Package::Current->InstalledLocation->Path->Data();
	_localfolderpath = Windows::Storage::ApplicationData::Current->LocalFolder->Path->Data();
	_cachefilepath = _localfolderpath / "DICOMVolumeDB";
	ReadCache();
	_MainFileSet = DICOMReader(_installfolderpath / dirname).MainFileSet;
	GatherVolumes();
	InitializeVolumes();
	LoadVolumes();
}

std::vector<char> DICOMLoader::LoadPointCloud() {
	std::vector<vert3> points = volumeset[0].GenerateIsoPointCloud(CorticalBone);
	char* data = (char*)points.data();
	vector<char> deserialized(data, data + points.size()*sizeof(vert3));
	return deserialized;
}

std::vector<char> DICOMLoader::LoadWireframeMesh() {
	std::vector<tri> Surface = MarchingCubes(volumeset[0].GetSamples(), HounsfieldScale[CorticalBone].lower);
	char* data = (char*)Surface.data();
	vector<char> deserialized(data, data + Surface.size()*sizeof(tri));
	return deserialized;
}

SceneTexture<Texture3D> DICOMLoader::LoadTexture3D(vector<task<void>>& tasks, shared_ptr<VanityCore>& vanitycore) {

	auto device = vanitycore->GetD3DDevice();

	size_t depth = volumeset[0].GetSamples().Depth();
	size_t width = volumeset[0].GetSamples().Width();
	size_t height = volumeset[0].GetSamples().Height();

	DXGI_FORMAT format;
	switch(sizeof(uint8_t)){
	case 1: format = DXGI_FORMAT_R8_UNORM; break;
	case 2: format = DXGI_FORMAT_R16_UNORM; break;
	default: format = DXGI_FORMAT_R8_UNORM;
	}

	auto texture3D = make_shared<Texture3D>(device, format, width, height, depth, 1,
											D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_IMMUTABLE);

	SceneTexture<Texture3D> scenetexture3D(texture3D);

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring FORUID = converter.from_bytes(volumeset[0].FORUID);
	std::wstring type = converter.from_bytes("CorticalBoneTexture");

	if (_caching) {
		std::filesystem::path textureFilename(QueryCache(FORUID, type));
		if (std::filesystem::exists(textureFilename)) {
			scenetexture3D.Load(tasks, textureFilename);
			return scenetexture3D;
		}
	}

	auto VolumeTexture = volumeset[0].GenerateIsoSamples(CorticalBone);

	if (_caching) {
		std::wstring filepath(Windows::Storage::ApplicationData::Current->LocalFolder->Path->Data());
		filepath.append(L"\\NewVolumeTexture");
		WriteTexture3D(VolumeTexture, filepath);
		_filecache.push_back({ FORUID, type, filepath.c_str()});
		WriteCache();
	}

	uint8_t* texturedata = VolumeTexture.Points();
	scenetexture3D.Load(tasks, vector<uint8_t>(texturedata, texturedata + depth * width * height));

	return scenetexture3D;
}
