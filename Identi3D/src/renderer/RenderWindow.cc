//
// File: RenderWindow.cc
// =====================
//

#include <src/renderer/RenderWindow.h>
#include <src/renderer/RenderDevice.h>
#include <src/identi3d/System.h>
#include <src/identi3d/EventDispatcher.h>
#include <src/utils/DebugManager.h>

namespace Identi3D
{
	
	const LONG window_style	= (	WS_OVERLAPPED	| \
								WS_CAPTION		| \
								WS_SYSMENU		| \
								WS_MINIMIZEBOX);

	RenderWindow::RenderWindow()
	{
		window = NULL;
		debugger = NULL;
		generateClassName();
	}

	RenderWindow::~RenderWindow(void)
	{
		release();
	}

	HRESULT RenderWindow::assign(RenderDevice *device, const TCHAR *title)
	{
		HRESULT hr;

		release();
		if(device == NULL || title == NULL) {
			_DebugPrint(debugger, E_INVALID_PARAMETERS);
			return E_FAIL;
		}
		registerClass();
		hr = createWindow(device->getWidth(), device->getHeight(), title);
		if(FAILED(hr))
			return E_FAIL;
		render_device = device;

		return S_OK;
	}

	HRESULT RenderWindow::release(void)
	{
		if(window != NULL) {
			DestroyWindow(window);
			window = NULL;
		}
		return S_OK;
	}

	void RenderWindow::generateClassName(void)
	{
		const TCHAR random_table[37] = __T("abcdefghijklmnopqrstuvwxyz0123456789");
		const UINT random_length = 7;

		_tcscpy_s(class_name, __T("NRW"));
		TCHAR *p = class_name + 3;
		srand((unsigned int)time(NULL));
		for(int i = 0; i < random_length; i++) {
			*p = random_table[rand() % 36];
			p++;
		}
		*p = __T('\0');

		return ;
	}

	void RenderWindow::registerClass(void)
	{
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= (WNDPROC)WindowProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= GetModuleHandle(NULL);
		wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= class_name;
		wcex.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);

		RegisterClassEx(&wcex);
	}

	HRESULT RenderWindow::createWindow(int width, int height, const TCHAR *title)
	{
		window = CreateWindow(
			class_name, title, window_style, 
			CW_USEDEFAULT, 0, width, height, 
			NULL, NULL, GetModuleHandle(0), (LPVOID)this);
		
		if(window == NULL) {
			_DebugPrint(debugger, E_CREATE_RENDER_WINDOW_FAILURE);
			return E_FAIL;
		}

		ShowWindow(window, SW_NORMAL);
		UpdateWindow(window);
		return S_OK;
	}

	LRESULT RenderWindow::WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		return System::instance().getEventDispatcher()->postWindowMessage(wnd, msg, wparam, lparam);
	}

};