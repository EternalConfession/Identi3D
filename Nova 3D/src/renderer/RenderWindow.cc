//
// File: RenderWindow.cc
// =====================
//

#include "src/renderer/RenderWindow.h"
#include "src/renderer/RenderDevice.h"
#include "src/utils/DebugManager.h"

namespace Nova3D
{
	
	const LONG window_style	= (	WS_OVERLAPPED	| \
								WS_CAPTION		| \
								WS_SYSMENU		| \
								WS_MINIMIZEBOX);

	RenderWindow::RenderWindow(DebugManager *dbgmgr)
	{
		debug_manager = dbgmgr;
		window = NULL;
		generateClassName();
	}

	RenderWindow::~RenderWindow(void)
	{
		release();
	}

	HRESULT RenderWindow::assign(HWND wnd)
	{
		release();
		if(wnd == NULL) {
			_DEBUGPRINT(debug_manager, E_INVALID_PARAMETERS);
			return E_FAIL;
		}

		window = wnd;
		return S_OK;
	}

	HRESULT RenderWindow::assign(int width, int height, const TCHAR *title)
	{
		HRESULT hr;

		release();
		registerClass();
		if(title == NULL) {
			_DEBUGPRINT(debug_manager, E_INVALID_PARAMETERS);
			return E_FAIL;
		}
		hr = createWindow(width, height, title);
		if(FAILED(hr)) {
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT RenderWindow::assign(const RenderDevice *device, const TCHAR *title)
	{
		if(device == NULL) {
			_DEBUGPRINT(debug_manager, E_INVALID_PARAMETERS);
			return E_FAIL;
		}
		return assign(device->getWidth(), device->getHeight(), title);
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

		if(!RegisterClassEx(&wcex))
			while(true);
	}

	HRESULT RenderWindow::createWindow(int width, int height, const TCHAR *title)
	{
		window = CreateWindow(
			class_name, title, window_style, 
			CW_USEDEFAULT, 0, width, height, 
			NULL, NULL, GetModuleHandle(0), (LPVOID)this);
		
		if(window == NULL) {
			_DEBUGPRINT(debug_manager, E_CREATE_RENDER_WINDOW_FAILURE);
			return E_FAIL;
		}

		ShowWindow(window, SW_NORMAL);
		UpdateWindow(window);
		return S_OK;
	}
	
	int RenderWindow::start(void)
	{
		MSG msg;

		while(true) 
		{
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if(msg.message == WM_QUIT) break;
		}

		return static_cast<int>(msg.wParam);
	}

	LRESULT RenderWindow::WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		static RenderWindow *rw = NULL;
		CREATESTRUCT *cs;

		if(msg == WM_CREATE) {
			cs = reinterpret_cast<CREATESTRUCT *>(lparam);
			rw = static_cast<RenderWindow *>(cs->lpCreateParams);
		}
		if(rw != NULL)
			return rw->dispatchWindowEvent(msg, wparam, lparam);
		else
			return DefWindowProc(wnd, msg, wparam, lparam);
	}

	LRESULT RenderWindow::dispatchWindowEvent(UINT msg, WPARAM wparam, LPARAM lparam)
	{
		PAINTSTRUCT ps;
		HDC hdc;

		switch(msg)
		{
		case WM_PAINT:
			hdc = BeginPaint(window, &ps);
			EndPaint(window, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(window, msg, wparam, lparam);
		}

		return 0;
	}

};