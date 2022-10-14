#pragma once
#include "pch.h"

#include <Engine\Third Party\DirectX Tool Kit\VertexTypes.h>  
#include <Engine\Models\Mesh.h>

namespace vxe {

  class Box : public Mesh<DirectX::VertexPosition, uint16_t>{
      uint32_t samples_x, samples_y, samples_z;
      float norm_x, norm_y, norm_z;
      float samplingrate;
  public:
      Box(uint32_t samples_x, uint32_t samples_y, uint32_t samples_z, float dx, float dy, float dz) : 
          samples_x(samples_x), samples_y(samples_y), samples_z(samples_z)
      { 
        float width = dx * samples_x;
        float height = dy * samples_y;
        float depth = dz * samples_z;

		// normalize
        float& largest = width > height ? (width > depth ? width : depth) : (height > depth? height : depth);

        samplingrate = 1.0f/largest;
        norm_x = width * samplingrate;
        norm_y = height * samplingrate;
        norm_z = depth * samplingrate;

      }

    virtual concurrency::task<void> CreateAsync(_In_ ID3D11Device2* device) 
    {
      // DebugPrint("\t Box<VertexPosition> Vertex, uint16_t>::CreateAsync() ...\n");

      std::vector<DirectX::VertexPosition> vertices = {
        // front
        DirectX::VertexPosition(DirectX::XMFLOAT3(-norm_x/2, -norm_y/2, -norm_z/2)), 
        DirectX::VertexPosition(DirectX::XMFLOAT3(-norm_x/2,  norm_y/2, -norm_z/2)),
        DirectX::VertexPosition(DirectX::XMFLOAT3( norm_x/2,  norm_y/2, -norm_z/2)),
        DirectX::VertexPosition(DirectX::XMFLOAT3( norm_x/2, -norm_y/2, -norm_z/2)),

        // back
        DirectX::VertexPosition(DirectX::XMFLOAT3(-norm_x/2, -norm_y/2, norm_z/2)),
        DirectX::VertexPosition(DirectX::XMFLOAT3(-norm_x/2,  norm_y/2, norm_z/2)),
        DirectX::VertexPosition(DirectX::XMFLOAT3( norm_x/2,  norm_y/2, norm_z/2)),
        DirectX::VertexPosition(DirectX::XMFLOAT3( norm_x/2, -norm_y/2, norm_z/2)),

        // left
        DirectX::VertexPosition(DirectX::XMFLOAT3(-norm_x/2, -norm_y/2, -norm_z/2)),
        DirectX::VertexPosition(DirectX::XMFLOAT3(-norm_x/2,  norm_y/2, -norm_z/2)),
        DirectX::VertexPosition(DirectX::XMFLOAT3(-norm_x/2,  norm_y/2,  norm_z/2)),
        DirectX::VertexPosition(DirectX::XMFLOAT3(-norm_x/2, -norm_y/2,  norm_z/2)),

        // right
        DirectX::VertexPosition(DirectX::XMFLOAT3( norm_x/2, -norm_y/2, -norm_z/2)),
        DirectX::VertexPosition(DirectX::XMFLOAT3( norm_x/2,  norm_y/2, -norm_z/2)),
        DirectX::VertexPosition(DirectX::XMFLOAT3( norm_x/2,  norm_y/2,  norm_z/2)),
        DirectX::VertexPosition(DirectX::XMFLOAT3( norm_x/2, -norm_y/2,  norm_z/2)),

        // top
        DirectX::VertexPosition(DirectX::XMFLOAT3(-norm_x/2,  norm_y/2, -norm_z/2)),
        DirectX::VertexPosition(DirectX::XMFLOAT3(-norm_x/2,  norm_y/2,  norm_z/2)),
        DirectX::VertexPosition(DirectX::XMFLOAT3( norm_x/2,  norm_y/2,  norm_z/2)),
        DirectX::VertexPosition(DirectX::XMFLOAT3( norm_x/2,  norm_y/2, -norm_z/2)),

        // bottom
        DirectX::VertexPosition(DirectX::XMFLOAT3(-norm_x/2, -norm_y/2, -norm_z/2)),
        DirectX::VertexPosition(DirectX::XMFLOAT3(-norm_x/2, -norm_y/2,  norm_z/2)),
        DirectX::VertexPosition(DirectX::XMFLOAT3( norm_x/2, -norm_y/2,  norm_z/2)),
        DirectX::VertexPosition(DirectX::XMFLOAT3( norm_x/2, -norm_y/2, -norm_z/2))
      };

      std::vector<uint16_t> indices = {
          0, 1, 2, 
          0, 2, 3,

          4, 5, 6, 
          4, 6, 7,

          8, 9, 10, 
          8, 10, 11,

          12, 13, 14, 
          12, 14, 15,

          16, 17, 18, 
          16, 18, 19,

          20, 21, 22, 
          20, 22, 23,
      };

      return Mesh::CreateAsync(device, vertices, indices);
    }

    virtual concurrency::task<void> LoadAsync(_In_ ID3D11Device2* device, const std::wstring&) 
    {
      // DebugPrint("\t Box<PositionNormalVertex, unsigned>::CreateAsync() ...\n");

      std::vector <DirectX::VertexPosition> vertices;
      std::vector<uint16_t> indices;

      // Loading from a file

      return Mesh::CreateAsync(device, vertices, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    virtual concurrency::task<void> CreateAsync(_In_ ID3D11Device2* device, const std::vector<char>& data) 
    {
      // DebugPrint("\t Box<PositionNormalVertex, unsigned>::CreateAsync() ...\n");

      std::vector <DirectX::VertexPosition> vertices;
      std::vector<uint16_t> indices;

      // Loading from a file

      return Mesh::CreateAsync(device, vertices, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

  };

}
