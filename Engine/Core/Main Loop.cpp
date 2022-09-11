
//
//	Main Loop.cpp
//  Manages the pipeline and renderers, and runs the main loop
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "Utilities.h"

#include "Main Loop.h"
#include "Common\DirectXHelper.h"

using namespace vxe;
using namespace std;
using namespace DirectX;

using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
MainLoop::MainLoop(const std::shared_ptr<VanityCore>& deviceResources, const shared_ptr<IRenderer>& renderer3D) :
	_vanitycore(deviceResources), 
	_sceneRenderer(renderer3D)
{
	DebugPrint(string("\t MainLoop::Ctor... \n"));

	// Register to be notified if the Device is lost or recreated
	_vanitycore->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.
	_sceneRenderer->Initialize(_vanitycore);

	_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(_vanitycore));

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

MainLoop::~MainLoop()
{
	DebugPrint(string("\t MainLoop::Dtor... \n"));

	// Deregister device notification
	_vanitycore->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void MainLoop::CreateWindowSizeDependentResources()
{
	DebugPrint(string("\t MainLoop::CreateWindowSizeDependentResources() ...\n"));

	// TODO: Replace this with the size-dependent initialization of your app's content.
	_sceneRenderer->CreateWindowResources();
}

// Updates the application state once per frame.
void MainLoop::Update()
{
	__Once(DebugPrint(string("MainLoop::Update() ...\n")));

	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		_sceneRenderer->Update(m_timer);
		_fpsTextRenderer->Update(m_timer);
	});
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool MainLoop::Render()
{
	__Once(DebugPrint(string("MainLoop::Render() ...\n")));

	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0) return false;

	auto context = _vanitycore->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = _vanitycore->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { _vanitycore->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, _vanitycore->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	XMVECTORF32 color = { 0.14f, 0.14f, 0.14f, 1.0f };
	//XMVECTORF32 color = { 0.0f, 0.0f, 0.0f, 1.0f };
	context->ClearRenderTargetView(_vanitycore->GetBackBufferRenderTargetView(), color);
	context->ClearDepthStencilView(_vanitycore->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
	_sceneRenderer->Render();
	_fpsTextRenderer->Render();

	return true;
}

// Notifies renderers that device resources need to be released.
void MainLoop::OnDeviceLost()
{
	_sceneRenderer->ReleaseDeviceResources();
	_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void MainLoop::OnDeviceRestored()
{
	_sceneRenderer->CreateDeviceResources();
	_fpsTextRenderer->CreateDeviceDependentResources();

	CreateWindowSizeDependentResources();
}