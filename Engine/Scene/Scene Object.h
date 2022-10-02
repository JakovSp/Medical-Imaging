
//
//	Scene Object.h
//  Encapsulating an a scree object (model)
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "../Scene\Transforms\World Transforms.h"

#include "../Models/Mesh.h"

namespace vxe {

	template <typename T, typename U>
	class SceneObject {

	public:
		SceneObject() {}
		
		void SetMesh(std::shared_ptr<Mesh<T, U>>& mesh) { _mesh = mesh; }
		std::shared_ptr<Mesh<T, U>> GetMesh() { return _mesh; }

		void SetWorld(std::shared_ptr<WorldTransforms>& world) { _world = world; }
		virtual std::shared_ptr<WorldTransforms> GetWorld() { return _world; }

		void Bind(_In_ ID3D11DeviceContext2* context) {

			if (!_active) return;

			_mesh->BindVertexBuffer(context);
			_mesh->BindIndexBuffer(context);

			_world->Update(context);
			auto buffer = _world->GetConstantBuffer();
			buffer->Bind(context, ProgrammableStage::VertexShaderStage, _worldregister);
		}

		virtual void Draw(_In_ ID3D11DeviceContext2* context, bool indexed=true) {

			if (!_active) return;

			if(indexed) _mesh->DrawIndexed(context);
			else _mesh->Draw(context);
		}

		void DrawInstanced(_In_ ID3D11DeviceContext2* context, bool indexed=true) {
			if (indexed) _mesh->DrawIndexedInstanced(context);
			else _mesh->DrawInstanced(context);
		}

		void Acivate() { _active = true; }
		void Deactivate() { _active = false; }

		void Release() { 
			_mesh->Reset();
			_world->Reset();
		}

	protected:
		std::shared_ptr<Mesh<T, U>> _mesh;

		std::shared_ptr<WorldTransforms> _world;
		uint32_t _worldregister = 0;

		bool _active = true;
	};

}
