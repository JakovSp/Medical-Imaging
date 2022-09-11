#pragma once
#include "pch.h"

#include <Engine\Third Party\DirectX Tool Kit\VertexTypes.h>  
#include <Engine\Models\Mesh.h>

namespace vxe {

  template <typename T, typename U> class Wireframe : public Mesh<T, U> {};

  template <>
  class Wireframe<DirectX::VertexPosition, uint16_t> : public Mesh<DirectX::VertexPosition, uint16_t>{

  public:
    Wireframe() { }

    virtual concurrency::task<void> CreateAsync(_In_ ID3D11Device2* device) 
    {
      // DebugPrint("\t Wireframe<Position> Vertex, uint16_t>::CreateAsync() ...\n");

		std::vector<DirectX::VertexPosition> vertices;
        std::vector<uint16_t> indices;

	  return Mesh::CreateAsync(device, vertices, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

	virtual concurrency::task<void> LoadAsync(_In_ ID3D11Device2* device, const std::wstring& filename) 
	{
        // _topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST
        return Mesh::LoadAsync(device, filename);
	}

	virtual concurrency::task<void> CreateAsync(_In_ ID3D11Device2* device, const std::vector<char>& data) 
	{
		std::vector <DirectX::VertexPosition> vertices;
		std::vector<uint16_t> indices;

		DirectX::VertexPosition* vertdata = (DirectX::VertexPosition*)data.data();
		vertices.assign(vertdata, vertdata + data.size()/(3*sizeof(float)));

		return Mesh::CreateAsync(device, vertices, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

  };

}
