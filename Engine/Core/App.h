
//
//	App.h
//  Defining an application view 
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#pragma once

#include "pch.h"

#include "Common\Vanity Core.h"
#include "Main Loop.h"

namespace vxe
{
	// Main entry point for our app. Connects the app with the Windows shell and handles application lifecycle events.
	ref class App sealed : public Windows::ApplicationModel::Core::IFrameworkView {

	internal:
		App(const std::shared_ptr<IRenderer>&);

	public:
		// IFrameworkView Methods.
		virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^);
		virtual void SetWindow(Windows::UI::Core::CoreWindow^ );
		virtual void Load(Platform::String^);
		virtual void Run();
		virtual void Uninitialize();

	protected:
		// Application lifecycle event handlers
		void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^,
			Windows::ApplicationModel::Activation::IActivatedEventArgs^);
		void OnSuspending(Platform::Object^ , Windows::ApplicationModel::SuspendingEventArgs^ );
		void OnResuming(Platform::Object^, Platform::Object^);

		// Window event handlers.
		void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^, Windows::UI::Core::WindowSizeChangedEventArgs^);
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^, Windows::UI::Core::VisibilityChangedEventArgs^);
		void OnWindowClosed(Windows::UI::Core::CoreWindow^, Windows::UI::Core::CoreWindowEventArgs^);

		// DisplayInformation event handlers.
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^, Platform::Object^);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^, Platform::Object^);
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^, Platform::Object^);

	private:
		std::shared_ptr<VanityCore> _vanitycore;
		std::unique_ptr<MainLoop> _main;

		bool m_windowClosed;
		bool m_windowVisible;

		std::shared_ptr<IRenderer> _renderer3D;
	};
}