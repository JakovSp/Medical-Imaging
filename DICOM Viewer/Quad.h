#pragma once
#include "pch.h"

#include <Engine\Third Party\DirectX Tool Kit\VertexTypes.h>  
#include <Engine\Models\Mesh.h>

namespace vxe {

  template <typename T, typename U> class Quad : public Mesh<T, U> {};

  template <>
  class Quad<DirectX::VertexPositionTexture3, uint16_t> : public Mesh<DirectX::VertexPositionTexture3, uint16_t>{

  public:
    Quad() { }

    virtual concurrency::task<void> CreateAsync(_In_ ID3D11Device2* device) 
    {
      // DebugPrint("\t PointCloud<Position3DTexture> Vertex, uint16_t>::CreateAsync() ...\n");

      std::vector<DirectX::VertexPositionTexture3> vertices = {
        DirectX::VertexPositionTexture3(DirectX::XMFLOAT3(-1.0f,-1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
        DirectX::VertexPositionTexture3(DirectX::XMFLOAT3(1.0f,-1.0f, -1.0f),  DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),
        DirectX::VertexPositionTexture3(DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f),  DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f)),
        DirectX::VertexPositionTexture3(DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f))
      };

      std::vector<uint16_t> indices = { 0, 3, 2,
                                        0, 2, 1,
                                        2, 3, 0,
                                        1, 2, 0
      };

      return Mesh::CreateAsync(device, vertices, indices);
    }

    virtual concurrency::task<void> LoadAsync(_In_ ID3D11Device2* device, const std::wstring&) 
    {
      // DebugPrint("\t PointCloud<PositionNormalVertex, unsigned>::CreateAsync() ...\n");

      std::vector <DirectX::VertexPositionTexture3> vertices;
      std::vector<uint16_t> indices;

      // Loading from a file

      return Mesh::CreateAsync(device, vertices, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    virtual concurrency::task<void> CreateAsync(_In_ ID3D11Device2* device, const std::vector<char>& data) 
    {
      // DebugPrint("\t PointCloud<PositionNormalVertex, unsigned>::CreateAsync() ...\n");

      std::vector <DirectX::VertexPositionTexture3> vertices;
      std::vector<uint16_t> indices;

      // Loading from a file

      return Mesh::CreateAsync(device, vertices, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

  };

}
