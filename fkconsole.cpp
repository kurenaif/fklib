#include "fkConsole.h"

namespace fk
{
	Console::Console(bool isShow) :m_isShow(isShow)
	{
		if (isShow) ::AllocConsole();
		FILE* out = 0; freopen_s(&out, "CON", "w", stdout);
		FILE* in = 0; freopen_s(&in, "CON", "r", stdin);
	}
}