#pragma once
#include "pch.h"

#include <Engine\Third Party\DirectX Tool Kit\VertexTypes.h>  
#include <Engine\Models\Mesh.h>

namespace vxe {
  class BoxSlice : public Mesh<DirectX::VertexIndex, uint16_t>{
  public:
      BoxSlice(size_t count) { 
          _instanced = true;
          _instancecount = count;
      }

    virtual concurrency::task<void> CreateAsync(_In_ ID3D11Device2* device) 
    {
      // DebugPrint("\t BoxSlice<VertexIndex> Vertex, uint16_t>::CreateAsync() ...\n");

      std::vector<DirectX::VertexIndex> vertices = {
          // Intersection indices
        DirectX::VertexIndex(0.0f, 0),
        DirectX::VertexIndex(1.0f, 0),
        DirectX::VertexIndex(2.0f, 0),
        DirectX::VertexIndex(3.0f, 0),
        DirectX::VertexIndex(4.0f, 0),
        DirectX::VertexIndex(5.0f, 0)
      };

      std::vector<uint16_t> indices = {
          // up to four triangles are necessary
          0, 1, 2,
          0, 2, 3,
          0, 3, 4,
          0, 4, 5
      };
      return Mesh::CreateAsync(device, vertices, indices);
    }

    virtual concurrency::task<void> LoadAsync(_In_ ID3D11Device2* device, const std::wstring&) 
    {
      // DebugPrint("\t BoxSlice<PositionNormalVertex, unsigned>::CreateAsync() ...\n");

      std::vector <DirectX::VertexIndex> vertices;
      std::vector<uint16_t> indices;

      // Loading from a file

      return Mesh::CreateAsync(device, vertices, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    virtual concurrency::task<void> CreateAsync(_In_ ID3D11Device2* device, const std::vector<char>& data) 
    {
      // DebugPrint("\t BoxSlice<PositionNormalVertex, unsigned>::CreateAsync() ...\n");

      std::vector <DirectX::VertexIndex> vertices;
      std::vector<uint16_t> indices;

      // Loading from a file

      return Mesh::CreateAsync(device, vertices, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

  };

}
