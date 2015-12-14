/**
* @file fkapplicationBase.cpp
* @brief ApplicationBaseÇÃé¿ëï
* @author çgòT
* @date 2014/04/23
*/
#include "fkapplicationBase.h"
namespace fk
{
	ApplicationBase::ApplicationBase(
		const tstring &windowName,
		const int &nCmdShow,
		const UINT &width, const UINT &height,
		const int &position_x, const int &position_y,
		const DWORD &style,
		const HICON &hIcon,
		const HCURSOR &hCursor,
		const tstring &className,
		const int &limit)
	{
		tstring aClassName;
		if (windowName != TEXT("fkwindow") || className == TEXT("fkwindow")) aClassName = windowName;
		else aClassName = className;
		bool isFind = true;
		for (int i = 0; i < 100; ++i)
		{
			tstring str = (Tstr() << aClassName << i).tstr();
			if (FindWindow(str.c_str(), NULL) == NULL)
			{
				isFind = false;
				aClassName = str;
				break;
			}
		}
		if (isFind) throw("Window Create limit.Please input limit value");
		window.Init(GetModuleHandle(NULL),aClassName, nCmdShow, windowName, width, height, position_x, position_y, style, hIcon, hCursor);
	}

	bool ApplicationBase::Run()
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				::PostQuitMessage(NULL);
				return false;
			}
		}
		if (IsWindow(window.GetHWND())) return true;
		else return false;
	}
}