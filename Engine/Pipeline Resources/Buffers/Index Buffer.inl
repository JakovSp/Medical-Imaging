
// 
//	Index Buffer.inl
//  Encapsulating a vertex buffer object
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

template <typename T>
IndexBuffer<T>::IndexBuffer(_In_ ID3D11Device2 *device, void* indices, int32_t count,
	D3D11_USAGE usage,
	uint32_t access,
	uint32_t flags,
	uint32_t stride) :
	_indexsize{ sizeof(T) },
	BufferBase{ sizeof(T) * count }
{
	DebugPrint(std::string("\t IndexBuffer<") + std::string(typeid(T).name()) + std::string(">::Ctor ...\n"));
	DebugPrint(std::string("\t\t Index Size: ") + std::to_string(sizeof(T)) + std::string("\n"));
	DebugPrint(std::string("\t\t Index Count: ") + std::to_string(count) + std::string("\n"));
	DebugPrint(std::string("\t\t Buffer Size: ") + std::to_string(_size) + std::string("\n"));

	//assert(_buffer != 0);

	CD3D11_BUFFER_DESC desc(_size, D3D11_BIND_INDEX_BUFFER, usage, access, flags, stride);

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = indices;

	//data.SysMemPitch = 0;
	//data.SysMemSlicePitch = 0;

	ThrowIfFailed(device->CreateBuffer(
		&desc, 
		&data, 
		_buffer.ReleaseAndGetAddressOf()), __FILEW__, __LINE__);
}