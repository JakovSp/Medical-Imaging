#pragma once
#include "pch.h"

#include <Engine\Third Party\DirectX Tool Kit\VertexTypes.h>  
#include <Engine\Models\Mesh.h>

namespace vxe {

	enum CubeFace {
        Front, Back, Left, Right, Top, Bottom
	};

  class ObjectAlignedSlices : public Mesh<DirectX::VertexPositionTextureInstanced, uint16_t>{
      uint32_t samples_x, samples_y, samples_z;
      float norm_x, norm_y, norm_z;
      float samplingrate;
  public:
      ObjectAlignedSlices(uint32_t samples_x, uint32_t samples_y, uint32_t samples_z, float dx, float dy, float dz) : 
          samples_x(samples_x), samples_y(samples_y), samples_z(samples_z)
      { 
        _instanced = true;
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
      // DebugPrint("\t ObjectAlignedSlices<VertexPositionTextureInstanced> Vertex, uint16_t>::CreateAsync() ...\n");

      std::vector<DirectX::VertexPositionTextureInstanced> vertices = {
        // front
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4(-norm_x/2, -norm_y/2, -norm_z/2, samplingrate),  DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4(-norm_x/2,  norm_y/2, -norm_z/2, samplingrate),  DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2,  norm_y/2, -norm_z/2, samplingrate),  DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2, -norm_y/2, -norm_z/2, samplingrate),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)),

        // back
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4(-norm_x/2, -norm_y/2, norm_z/2, -samplingrate),  DirectX::XMFLOAT4(0.0f, 0.0f, 150.0f, -1.0f)),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4(-norm_x/2,  norm_y/2, norm_z/2, -samplingrate),  DirectX::XMFLOAT4(0.0f, 1.0f, 150.0f, -1.0f)),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2,  norm_y/2, norm_z/2, -samplingrate),  DirectX::XMFLOAT4(1.0f, 1.0f, 150.0f, -1.0f)),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2, -norm_y/2, norm_z/2, -samplingrate),  DirectX::XMFLOAT4(1.0f, 0.0f, 150.0f, -1.0f)),

        // left
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4(-norm_x/2, -norm_y/2, -norm_z/2, samplingrate),  DirectX::XMFLOAT4(0, 0.0f, 0.0f,  samplingrate )),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4(-norm_x/2,  norm_y/2, -norm_z/2, samplingrate),  DirectX::XMFLOAT4(0, 1.0f, 0.0f,  samplingrate )),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4(-norm_x/2,  norm_y/2,  norm_z/2, samplingrate),  DirectX::XMFLOAT4(0, 1.0f, samples_z, samplingrate )),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4(-norm_x/2, -norm_y/2,  norm_z/2, samplingrate),  DirectX::XMFLOAT4(0, 0.0f, samples_z, samplingrate )),

        // right
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2, -norm_y/2, -norm_z/2, -samplingrate),  DirectX::XMFLOAT4(samples_x, 0.0f, 0.0f,  -samplingrate )),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2,  norm_y/2, -norm_z/2, -samplingrate),  DirectX::XMFLOAT4(samples_x, 1.0f, 0.0f,  -samplingrate )),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2,  norm_y/2,  norm_z/2, -samplingrate),  DirectX::XMFLOAT4(samples_x, 1.0f, samples_z, -samplingrate )),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2, -norm_y/2,  norm_z/2, -samplingrate),  DirectX::XMFLOAT4(samples_x, 0.0f, samples_z, -samplingrate )),

        // top
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4(-norm_x/2,  norm_y/2, -norm_z/2, -samplingrate),  DirectX::XMFLOAT4(0.0f, samples_y, 0.0f, -samplingrate)),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4(-norm_x/2,  norm_y/2,  norm_z/2, -samplingrate),  DirectX::XMFLOAT4(0.0f, samples_y, samples_z, -samplingrate)),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2,  norm_y/2,  norm_z/2, -samplingrate),  DirectX::XMFLOAT4(1.0f, samples_y, samples_z, -samplingrate)),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2,  norm_y/2, -norm_z/2, -samplingrate),  DirectX::XMFLOAT4(1.0f, samples_y, 0.0f, -samplingrate)),

        // bottom
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4(-norm_x/2, -norm_y/2, -norm_z/2, samplingrate),  DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, samplingrate)),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4(-norm_x/2, -norm_y/2,  norm_z/2, samplingrate),  DirectX::XMFLOAT4(0.0f, 0.0f, samples_z, samplingrate)),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2, -norm_y/2,  norm_z/2, samplingrate),  DirectX::XMFLOAT4(1.0f, 0.0f, samples_z, samplingrate)),
        DirectX::VertexPositionTextureInstanced(DirectX::XMFLOAT4( norm_x/2, -norm_y/2, -norm_z/2, samplingrate),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, samplingrate))
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
        switch (_orientation) {
        case Front:
			context->DrawIndexedInstanced(6, samples_z, 6, 4*_orientation, 0);
            break;
        case Back:
			context->DrawIndexedInstanced(6, samples_z, 0, 4*_orientation, 0);
            break;
        case Right:
			context->DrawIndexedInstanced(6, samples_x, 6, 4*_orientation, 0);
            break;
        case Left:
			context->DrawIndexedInstanced(6, samples_x, 0, 4*_orientation, 0);
            break;
        case Top:
			context->DrawIndexedInstanced(6, samples_y, 6, 4*_orientation, 0);
            break;
        case Bottom:
			context->DrawIndexedInstanced(6, samples_y, 0, 4*_orientation, 0);
            break;
        }
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
