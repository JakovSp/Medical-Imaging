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
	//std::vector<float> vertices;
	converter.volumes[0].LoadVolume();
	Cloud3D<uint16_t> Tetrahedron(3,3,3);
	// Test values
	Tetrahedron[0][0][0] = 1200; Tetrahedron[0][0][1] = 1200; Tetrahedron[0][0][2] = 1200;
	Tetrahedron[0][1][0] = 1200; Tetrahedron[0][1][1] = 1200; Tetrahedron[0][1][2] = 1200;
	Tetrahedron[0][2][0] = 1200; Tetrahedron[0][2][1] = 1200; Tetrahedron[0][2][2] = 1200;
	
	Tetrahedron[1][0][0] = 1200; Tetrahedron[1][0][1] = 1200; Tetrahedron[1][0][2] = 1200;
	Tetrahedron[1][1][0] = 1200; Tetrahedron[1][1][1] = 1800; Tetrahedron[1][1][2] = 1200;
	Tetrahedron[1][2][0] = 1200; Tetrahedron[1][2][1] = 1200; Tetrahedron[1][2][2] = 1200;
	
	Tetrahedron[2][0][0] = 1200; Tetrahedron[2][0][1] = 1200; Tetrahedron[2][0][2] = 1200;
	Tetrahedron[2][1][0] = 1200; Tetrahedron[2][1][1] = 1200; Tetrahedron[2][1][2] = 1200;
	Tetrahedron[2][2][0] = 1200; Tetrahedron[2][2][1] = 1200; Tetrahedron[2][2][2] = 1200;

	//std::vector<vert3> points = converter.volumes[0].GenerateIsoPointCloud(CorticalBone);
	std::vector<vert3> points = converter.volumes[0].GenerateIsoPointCloud(CorticalBone);
	char* data = (char*)points.data();
	vector<char> deserialized(data, data + points.size()*sizeof(vert3));
	return deserialized;
}


std::vector<char> DICOMLoader::LoadWireframeMesh() {

	converter.volumes[0].LoadVolume();
//	// Test values
//	Cloud3D<uint16_t> Tetrahedron(3,3,3);
//	Tetrahedron[0][0][0] = 1200; Tetrahedron[0][0][1] = 1200; Tetrahedron[0][0][2] = 1200;
//	Tetrahedron[0][1][0] = 1200; Tetrahedron[0][1][1] = 1200; Tetrahedron[0][1][2] = 1200;
//	Tetrahedron[0][2][0] = 1200; Tetrahedron[0][2][1] = 1200; Tetrahedron[0][2][2] = 1200;
//	
//	Tetrahedron[1][0][0] = 1200; Tetrahedron[1][0][1] = 1200; Tetrahedron[1][0][2] = 1200;
//	Tetrahedron[1][1][0] = 1200; Tetrahedron[1][1][1] = 1800; Tetrahedron[1][1][2] = 1200;
//	Tetrahedron[1][2][0] = 1200; Tetrahedron[1][2][1] = 1200; Tetrahedron[1][2][2] = 1200;
//	
//	Tetrahedron[2][0][0] = 1200; Tetrahedron[2][0][1] = 1200; Tetrahedron[2][0][2] = 1200;
//	Tetrahedron[2][1][0] = 1200; Tetrahedron[2][1][1] = 1200; Tetrahedron[2][1][2] = 1200;
//	Tetrahedron[2][2][0] = 1200; Tetrahedron[2][2][1] = 1200; Tetrahedron[2][2][2] = 1200;
//	std::vector<tri> Surface = MarchingCubes(Tetrahedron, 1500);
	std::vector<tri> Surface = MarchingCubes(converter.volumes[0].Volume, HounsfieldScale[CorticalBone].lower);

	char* data = (char*)Surface.data();
	vector<char> deserialized(data, data + Surface.size()*sizeof(tri));
	return deserialized;
}

SceneTexture<Texture3D> DICOMLoader::LoadTexture3D(vector<task<void>>& tasks, shared_ptr<VanityCore>& vanitycore) {
	converter.volumes[0].LoadVolume();
	auto VolumeTexture = converter.volumes[0].GenerateWindowedVolume();
	size_t depth = VolumeTexture.Depth();
	size_t width = VolumeTexture.Width();
	size_t height = VolumeTexture.Height();

	auto device = vanitycore->GetD3DDevice();
	auto tex3D = make_shared<Texture3D>(device, DXGI_FORMAT_R8_UNORM, width, height, depth, 1,
										D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_IMMUTABLE);

	SceneTexture<Texture3D> scenetex3D(tex3D);
	//scenetex3D.Load(tasks, VolumeTexture.Points());

	return scenetex3D;
}

SceneTexture<Texture2D> DICOMLoader::LoadTextureArray(vector<task<void>>& tasks, shared_ptr<VanityCore>& vanitycore) {
	converter.volumes[0].LoadVolume();
	auto VolumeTexture = converter.volumes[0].GenerateWindowedVolume();
	size_t depth = VolumeTexture.Depth();
	size_t width = VolumeTexture.Width();
	size_t height = VolumeTexture.Height();

	auto device = vanitycore->GetD3DDevice();
	auto texarray = make_shared<Texture2D>(device, DXGI_FORMAT_R8_UNORM, width, height, depth, 1,
										D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_IMMUTABLE);

	SceneTexture<Texture2D> scenetexarray(texarray);
	uint8_t* texdata = (uint8_t*)VolumeTexture.Points();
	scenetexarray.Load(tasks, std::vector<uint8_t>(texdata, texdata + width*height*depth));

	return scenetexarray;
}
