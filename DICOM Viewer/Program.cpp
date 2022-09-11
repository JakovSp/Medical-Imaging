
//
//	Program.cpp
//  Testing Vanity shapes
// 
//	Objectives:
//	(1) Add Include Dir to ../ 
//	(2) Add Library via the Right Mouse on the Project -> Add -> References ...
//  (3) Going Fullscreen: Shift + Win + Enter
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "Renderer.h"

using namespace Platform;

using namespace vxe;
using namespace std;

[MTAThread]
int32_t main(Array<String^>^)
{
	DebugPrint(string("main() started ...\n"));

	Vanity vanity{ make_shared<Renderer>() };
	vanity.Run();

	return 0;
}
