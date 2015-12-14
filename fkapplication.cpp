#include "fkapplication.h"


namespace fk
{
	Application::Application(const tstring &windowName,
		const int &nCmdShow,
		const UINT &width, const UINT &height,
		const int &position_x, const int &position_y,
		const DWORD &style,
		const HICON &hIcon,
		const HCURSOR &hCursor,
		const tstring &className) :ApplicationBase(windowName, nCmdShow, width, height, position_x, position_y, style, hIcon, hCursor, className), m_key(new Input), m_keyBefore(new Input)
	{
	}

	bool Application::Run()
	{
		std::swap(m_key, m_keyBefore);
		m_key->Update();
		//メッセージループここから
		return ApplicationBase::Run();
	}

	bool Application::IsKeyPush(const int &key)
	{
		return (((!m_keyBefore->GetKeyState(key)) && m_key->GetKeyState(key)) ? true : false);
	}

	bool Application::IsKeyPull(const int &key)
	{
		return (m_keyBefore->GetKeyState(key) && (!m_key->GetKeyState(key)) ? true : false);
	}	
}
