/**
* @file fkwindowbase.cpp
* @brief windowbaseクラスの実装
* @author 紅狼
*/

#include "fkwindowbase.h"

namespace fk
{
	WindowBase::WindowBase()
	{
		m_wCEx.cbSize			= sizeof(WNDCLASSEX);
		m_wCEx.lpfnWndProc		= WndProc;
		m_wCEx.style			= (CS_HREDRAW | CS_VREDRAW);
		m_wCEx.cbClsExtra		= 0;
		m_wCEx.cbWndExtra		= 0;
		m_wCEx.hInstance		= NULL;
		m_wCEx.hIcon			= NULL;
		m_wCEx.hCursor			= LoadCursor(NULL, IDC_ARROW);
		m_wCEx.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
		m_wCEx.lpszMenuName		= NULL;
		m_wCEx.lpszClassName	= NULL;
		m_wCEx.hIconSm			= NULL;

		m_hWnd = NULL;
	}

	WindowBase::WindowBase(const HINSTANCE hInst,const tstring &className,
		const int &nCmdShow,
		const tstring &windowName,
		const UINT &width, const UINT &height,
		const int &position_x, const int &position_y,
		const DWORD &style,
		const HICON &hIcon,
		const HCURSOR &hCursor)
	{
		Init(hInst, className, nCmdShow, windowName, width, height, position_x, position_y, style, hIcon, hCursor);
	}

	void WindowBase::Init(const HINSTANCE hInst,
		const tstring &className,
		const int &nCmdShow,
		const tstring &windowName,
		const UINT &width, const UINT &height,
		const int &position_x, const int &position_y,
		const DWORD &style,
		const HICON &hIcon,
		const HCURSOR &hCursor)
	{
		m_wCEx.cbSize = sizeof(WNDCLASSEX);
		m_wCEx.lpfnWndProc = WndProc;
		m_wCEx.style = (CS_HREDRAW | CS_VREDRAW);
		m_wCEx.cbClsExtra = 0;
		m_wCEx.cbWndExtra = 0;
		m_wCEx.hInstance = hInst;
		m_wCEx.hIcon = hIcon;
		m_wCEx.hCursor = hCursor;
		m_wCEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		m_wCEx.lpszMenuName = NULL;
		m_wCEx.lpszClassName = className.c_str();
		m_wCEx.hIconSm = NULL;

		if (RegisterClassEx(&m_wCEx) == 0) throw("RegisterClassEx() failed");

		int windowWidth = CW_USEDEFAULT;
		int windowHeight = CW_USEDEFAULT;

		if (width != CW_USEDEFAULT)
		{
			//windowsizeの調整
			RECT rect = { 0, 0, width, height };
			AdjustWindowRect(&rect, style, false);
			windowWidth = rect.right - rect.left;
			windowHeight = rect.bottom - rect.top;
		}

		m_hWnd = CreateWindowEx(0, className.c_str(), windowName.c_str(), style, position_x, position_y, windowWidth, windowHeight, NULL, NULL, hInst, NULL);
		if (m_hWnd == NULL) throw("CreateWindowEx() failed");

		ShowWindow(m_hWnd, nCmdShow);
		UpdateWindow(m_hWnd);
	}

	WindowBase::~WindowBase()
	{
		UnregisterClass(m_wCEx.lpszClassName, m_wCEx.hInstance);
	}

	//コールバック
	LRESULT CALLBACK WindowBase::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hinstPrev, LPSTR lpszCmdLine, int nCmdShow)
{
	fk::main(hinst);
}
