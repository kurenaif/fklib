#include "fkDxApp.h"
#include "fkConsole.h"
#include <iostream>
#include <math.h>

int fk::main(HINSTANCE hInst)
{
	fk::DxApp app(TEXT("window"), fk::Size<int>(640,480));
	fk::Console console;
	unsigned int a=0;
	bool b = true;
	while (app.Run())
	{
		//app.SpriteBegin();
		//app.Clear(0, 0, 0, 0);

		//tstring name = TEXT("image.jpg");
		//app.AddTexture(name);

		//app.SpriteBegin(SpriteSortMode_Deferred);
		//app.SPriteDrawTexture(name, XMFLOAT2(0, 0), nullptr);
		//app.SpriteEnd();
	}
	return 0;
}
