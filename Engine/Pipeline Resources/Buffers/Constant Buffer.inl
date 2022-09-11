
//
//	Constant Buffer.inl
//  Encapsulating a constant buffer (a structure which size is multiple of 16 bytes)
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

template <typename T>
ConstantBuffer<T>::ConstantBuffer(_In_ ID3D11Device2* device, T* data, D3D11_USAGE usage,
	uint32_t access,
	uint32_t flags,
	uint32_t stride) : 
	BufferBase(16 * ((sizeof(T) + 15) / 16))
{
	DebugPrint(std::string("\t ConstantBuffer<") + typeid(T).name() + std::string(">::Ctor ...\n"));
	DebugPrint(std::string("\t\t Buffer Size: ") + std::to_string(_size) + std::string("\n"));

	data = nullptr;

	//	assert(_buffer != 0);

	// For constant buffers the byte width must be multiple of 16 bytes
	// The maximum constant buffer size that is supported by shaders is 4096 128-bit constants
	CD3D11_BUFFER_DESC desc(_size, D3D11_BIND_CONSTANT_BUFFER, usage, access, flags, stride);

	ThrowIfFailed(device->CreateBuffer(
		&desc,
		nullptr,
		&_buffer), __FILEW__, __LINE__);
}

template <typename T>
void ConstantBuffer<T>::Bind(_In_ ID3D11DeviceContext2* context, ProgrammableStage shader, uint32_t slot, uint32_t count)
{
	//DebugPrint(std::string("\t ConstantBuffer<") + typeid(T).name() + std::string(">::Bind() ...\n"));

	//	A constant buffer can be bound to 6 pipeline locations
	//	TODO: See methods *SSet*Buffers1
	if (shader == ProgrammableStage::VertexShaderStage) {
		context->VSSetConstantBuffers(slot, count, _buffer.GetAddressOf());
	}
	else if (shader == ProgrammableStage::HullShaderStage) {
		context->HSSetConstantBuffers(slot, count, _buffer.GetAddressOf());
	}
	else if (shader == ProgrammableStage::DomainShaderStage) {
		context->DSSetConstantBuffers(slot, count, _buffer.GetAddressOf());
	}
	else if (shader == ProgrammableStage::GeometryShaderStage) {
		context->GSSetConstantBuffers(slot, count, _buffer.GetAddressOf());
	}
	else if (shader == ProgrammableStage::PixelShaderStage) {
		context->PSSetConstantBuffers(slot, count, _buffer.GetAddressOf());
	}
	else if (shader == ProgrammableStage::ComputeShaderStage) {
		context->CSSetConstantBuffers(slot, count, _buffer.GetAddressOf());
	}
	else
		return;
}