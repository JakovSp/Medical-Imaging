#include "pch.h"
#include "DICOMLoader.h"

#include <DICOM Reader/Convert/Geometry/Geometry.h>
#include <DICOM Reader/Convert/Geometry/Volume.h>
#include <DICOM Reader/Convert/Geometry/MarchingCubes.h>

using namespace std;
using namespace vxe;
using namespace DirectX;

using namespace concurrency;
using namespace vxe::med;
using namespace vxe::geo;

std::vector<char> DICOMLoader::LoadPointCloud() {
	volumes[0].LoadVolume();
	std::vector<vert3> points = volumes[0].GenerateIsoPointCloud(CorticalBone);
	char* data = (char*)points.data();
	vector<char> deserialized(data, data + points.size()*sizeof(vert3));
	return deserialized;
}

std::vector<char> DICOMLoader::LoadWireframeMesh() {
	volumes[0].LoadVolume();
	std::vector<tri> Surface = MarchingCubes(volumes[0].GetSamples(), HounsfieldScale[CorticalBone].lower);
	char* data = (char*)Surface.data();
	vector<char> deserialized(data, data + Surface.size()*sizeof(tri));
	return deserialized;
}

SceneTexture<Texture3D> DICOMLoader::LoadTexture3D(vector<task<void>>& tasks, shared_ptr<VanityCore>& vanitycore) {
	volumes[0].LoadVolume();
	auto VolumeTexture = volumes[0].GenerateWindowedSamples();
	size_t depth = VolumeTexture.Depth();
	size_t width = VolumeTexture.Width();
	size_t height = VolumeTexture.Height();

	auto device = vanitycore->GetD3DDevice();
	auto texture3D = make_shared<Texture3D>(device, DXGI_FORMAT_R8_UNORM, width, height, depth, 1,
										D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_IMMUTABLE);

	SceneTexture<Texture3D> scenetexture3D(texture3D);
	uint8_t* texturedata = VolumeTexture.Points();
	scenetexture3D.Load(tasks, vector<uint8_t>(texturedata, texturedata + depth * width * height));

	return scenetexture3D;
}
