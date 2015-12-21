#pragma once

#include "SDL_keysym.h"
#include "sdl.h"

enum KeyStates {
	KEY_NONE,
	KEY_PRESSED,
	KEY_RELEASED,
	KEY_HELD
};

class Keyboard
{
public:
	Keyboard(void);
	~Keyboard(void);

	void ProcessEvent(SDL_Event &a_event);

	KeyStates GetKeyStates(SDLKey a_key);

	int DetectKeysReleased();
	int DetectOneKeyReleased(SDLKey a_key);

	void flushAllKeys();

private:
	KeyStates m_keys[SDLK_LAST];
};

