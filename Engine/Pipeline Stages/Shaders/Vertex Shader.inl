
//
//	Vertex Shader.inl
//  Encapsulating a vertex shader object
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

template <typename T>
VertexShader<T>::VertexShader() :
	_shader{ nullptr },
	_layout{ nullptr }
{}

template <typename T>
concurrency::task<void> VertexShader<T>::CreateAsync(_In_ ID3D11Device2* device, const std::wstring& filename)
{
	DebugPrint(string("\t VertexShader<") + typeid(T).name() + string(">::CreateAsync() ...\n"));

	return concurrency::create_task(DX::ReadDataAsync(filename)).then([this, device](const vector<uint8_t> data) {
		DebugPrint(std::string("\t -- A lambda: Creating a VS and an IL \n"));

		ThrowIfFailed(device->CreateVertexShader(
			&data[0],
			data.size(),
			nullptr,
			&_shader), __FILEW__, __LINE__);

		ThrowIfFailed(device->CreateInputLayout(
			T::InputElements,
			T::InputElementCount,
			&data[0], data.size(),
			&_layout), __FILEW__, __LINE__);

		DebugPrint(std::string("\t -- A lambda: a VS and an IL created. \n"));
		});
}

template <typename T>
void VertexShader<T>::Bind(_In_ ID3D11DeviceContext2* context, ID3D11ClassInstance* const* instances, uint32_t count)
{
	//DebugPrint(std::string("\t VertexShader<") + typeid(T).name() + std::string(">::Bind() ...\n"));

	context->VSSetShader(_shader.Get(), instances, count);
	context->IASetInputLayout(_layout.Get());
}

template <typename T>
void VertexShader<T>::Reset()
{
	DebugPrint(std::string("\t VertexShader<") + typeid(T).name() + std::string(">::Reset() ...\n"));

	_shader.Reset();
	_layout.Reset();
}