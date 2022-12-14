
#pragma once

//
//	Vanity Core.h
//  Defining a type for managing the pipeline core objects
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

namespace vxe
{
    // Provides an interface for an application that owns DeviceResources to be notified of the device being lost or created.
	interface IDeviceNotify
	{
		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceRestored() = 0;
	};

	// A Facade: Controls all the DirectX device resources.
	class VanityCore {

	public:
		VanityCore();
		void SetWindow(Windows::UI::Core::CoreWindow^ window);
		void SetLogicalSize(Windows::Foundation::Size logicalSize);
		void SetCurrentOrientation(Windows::Graphics::Display::DisplayOrientations currentOrientation);
		void SetDpi(float dpi);
		void ValidateDevice();
		void HandleDeviceLost();
		void RegisterDeviceNotify(IDeviceNotify* deviceNotify);
		void Trim();
		void Present();

		ID3D11RasterizerState1* CreateRasterizerState(D3D11_FILL_MODE, D3D11_CULL_MODE, bool);

		void SetRasterizerState();

		void SetRasterizerState(ID3D11RasterizerState1* state) { m_d3dContext->RSSetState(state); }
		auto GetRasterizerState() const { return _rasterizerstate.Get(); }

		void SetBlenderState();
		auto GetBlenderState() const { return _blendstate.Get(); }

		// Device Accessors.
		auto GetOutputSize() const { return m_outputSize; }
		auto GetLogicalSize() const	{ return m_logicalSize; }

		// D3D Accessors.
		auto GetD3DDevice() const { return m_d3dDevice.Get(); }
		auto GetD3DDeviceContext() const { return m_d3dContext.Get(); }
		auto GetSwapChain() const { return m_swapChain.Get(); }
		auto GetDeviceFeatureLevel() const { return m_d3dFeatureLevel; }
		auto GetBackBufferRenderTargetView() const { return m_d3dRenderTargetView.Get(); }
		auto GetDepthStencilView() const { return m_d3dDepthStencilView.Get(); }
		auto GetScreenViewport() const { return m_screenViewport; }
		auto GetOrientationTransform3D() const { return m_orientationTransform3D; }

		// D2D Accessors.
		auto GetD2DFactory() const{ return m_d2dFactory.Get(); }
		auto GetD2DDevice() const { return m_d2dDevice.Get(); }
		auto GetD2DDeviceContext() const { return m_d2dContext.Get(); }
		auto GetD2DTargetBitmap() const { return m_d2dTargetBitmap.Get(); }
		auto GetDWriteFactory() const { return m_dwriteFactory.Get();	 }
		auto GetWicImagingFactory() const { return m_wicFactory.Get(); }
		auto GetOrientationTransform2D() const { return m_orientationTransform2D; }

	private:
		void CreateDeviceIndependentResources();
		void CreateDeviceResources();
		void CreateWindowSizeDependentResources();
		DXGI_MODE_ROTATION ComputeDisplayRotation();

		// Direct3D objects.
		Microsoft::WRL::ComPtr<ID3D11Device2> m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext2> m_d3dContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;

		// Direct3D rendering objects. Required for 3D.
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_d3dRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_d3dDepthStencilView;
		D3D11_VIEWPORT m_screenViewport;

		// Direct2D drawing components.
		Microsoft::WRL::ComPtr<ID2D1Factory2> m_d2dFactory;
		Microsoft::WRL::ComPtr<ID2D1Device1> m_d2dDevice;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext1> m_d2dContext;
		Microsoft::WRL::ComPtr<ID2D1Bitmap1>m_d2dTargetBitmap;

		// DirectWrite drawing components.
		Microsoft::WRL::ComPtr<IDWriteFactory2>	m_dwriteFactory;
		Microsoft::WRL::ComPtr<IWICImagingFactory2>	m_wicFactory;

		// Cached reference to the Window.
		Platform::Agile<Windows::UI::Core::CoreWindow> m_window;

		// Cached device properties.
		D3D_FEATURE_LEVEL m_d3dFeatureLevel;
		Windows::Foundation::Size m_d3dRenderTargetSize;
		Windows::Foundation::Size m_outputSize;
		Windows::Foundation::Size m_logicalSize;
		Windows::Graphics::Display::DisplayOrientations	m_nativeOrientation;
		Windows::Graphics::Display::DisplayOrientations	m_currentOrientation;
		float m_dpi;

		// Transforms used for display _orientation.
		D2D1::Matrix3x2F m_orientationTransform2D;
		DirectX::XMFLOAT4X4	m_orientationTransform3D;

		// The IDeviceNotify can be held directly as it owns the DeviceResources.
		IDeviceNotify* m_deviceNotify;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState1> _rasterizerstate;
		Microsoft::WRL::ComPtr<ID3D11BlendState1> _blendstate;
	};

}