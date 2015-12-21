// GameEngine.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"

#include "SDL.h"
#include "GameController.h"
#include "Sprite.h"
#include "settings.h"



int _tmain(int argc, _TCHAR* argv[])
{

	GameController *pApp = new GameController(1024, SCREEN_HEIGHT, false);

	pApp->Run();

	delete pApp;

	return 0;
}

