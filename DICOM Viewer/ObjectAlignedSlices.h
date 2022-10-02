#pragma once
#include "pch.h"

#include <Engine\Third Party\DirectX Tool Kit\VertexTypes.h>  
#include <Engine\Models\Mesh.h>

namespace vxe {

	enum CubeFace {
		Front = 0, Right = 1, Top = 2
	};


  class ObjectAlignedSlices : public Mesh<DirectX::VertexPositionTextureInstanced, uint16_t>{
      float norm_x, norm_y, norm_z;
      float norm_dx, norm_dy, norm_dz;
  public:
      ObjectAlignedSlices(uint32_t width, uint32_t height, uint32_t depth, float dx, float dy, float dz) { 
          _instanced = true;
          _instancecount = depth;
        float length_x = dx * width;
        float length_y = dy * height;
        float length_z = dz * depth;

		// normalize
        float& largest = length_x > length_y ? (length_x > length_z ? length_x : length_z) : (length_y > length_z? length_y : length_z);

        norm_x = length_x / largest;
        norm_y = length_y / largest;
        norm_z = length_z / largest;
        norm_dx = dx / width;
        norm_dy = dy / height;
        norm_dz = dz / depth;
      }

    virtual concurrency::task<void> CreateAsync(_In_ ID3D11Device2* device) 
    {
      // DebugPrint("\t ObjectAlignedSlices<VertexPositionTextureInstanced> Vertex, uint16_t>::CreateAsync() ...\n");

      std::vector<DirectX::VertexPositionTextureInstanced> vertices = {
        // front
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4(-norm_x/2, -norm_y/2, -norm_z/2, norm_dx),  DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4(-norm_x/2,  norm_y/2, -norm_z/2, norm_dx),  DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2,  norm_y/2, -norm_z/2, norm_dx),  DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2, -norm_y/2, -norm_z/2, norm_dx),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)),

        // right
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2, -norm_y/2, -norm_z/2, -norm_dz),  DirectX::XMFLOAT4(_instancecount, 0.0f, 0.0f,   -norm_dz )),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2,  norm_y/2, -norm_z/2, -norm_dz),  DirectX::XMFLOAT4(_instancecount, 1.0f, 0.0f,   -norm_dz )),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2,  norm_y/2,  norm_z/2, -norm_dz),  DirectX::XMFLOAT4(_instancecount, 1.0f, _instancecount, -norm_dz )),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2, -norm_y/2,  norm_z/2, -norm_dz),  DirectX::XMFLOAT4(_instancecount, 0.0f, _instancecount, -norm_dz )),

        // top
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4(-norm_x/2,  norm_y/2, -norm_z/2, -norm_dz),  DirectX::XMFLOAT4(0.0f, _instancecount, 0.0f, -norm_dz)),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4(-norm_x/2,  norm_y/2,  norm_z/2, -norm_dz),  DirectX::XMFLOAT4(0.0f, _instancecount, _instancecount, -norm_dz)),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2,  norm_y/2,  norm_z/2, -norm_dz),  DirectX::XMFLOAT4(1.0f, _instancecount, _instancecount, -norm_dz)),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2,  norm_y/2, -norm_z/2, -norm_dz),  DirectX::XMFLOAT4(1.0f, _instancecount, 0.0f, -norm_dz))
      };

      std::vector<uint16_t> indices = {
          0, 1, 2, 
          0, 2, 3,
          0, 2, 1, 
          0, 3, 2
      };

      return Mesh::CreateAsync(device, vertices, indices);
    }

	virtual void Draw(_In_ ID3D11DeviceContext2* context, CubeFace _orientation) {
		context->DrawIndexedInstanced(12, _instancecount, 0, 4*_orientation, 0);
	}

    virtual concurrency::task<void> LoadAsync(_In_ ID3D11Device2* device, const std::wstring&) 
    {
      // DebugPrint("\t ObjectAlignedSlices<PositionNormalVertex, unsigned>::CreateAsync() ...\n");

      std::vector <DirectX::VertexPositionTextureInstanced> vertices;
      std::vector<uint16_t> indices;

      // Loading from a file

      return Mesh::CreateAsync(device, vertices, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    virtual concurrency::task<void> CreateAsync(_In_ ID3D11Device2* device, const std::vector<char>& data) 
    {
      // DebugPrint("\t ObjectAlignedSlices<PositionNormalVertex, unsigned>::CreateAsync() ...\n");

      std::vector <DirectX::VertexPositionTextureInstanced> vertices;
      std::vector<uint16_t> indices;

      // Loading from a file

      return Mesh::CreateAsync(device, vertices, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

  };

}
