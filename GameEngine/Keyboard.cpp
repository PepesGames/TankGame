#include "Keyboard.h"


Keyboard::Keyboard(void)
{
	memset(m_keys, KEY_NONE, sizeof(KeyStates)*SDLK_LAST);
}


Keyboard::~Keyboard(void)
{
}

void Keyboard::ProcessEvent(SDL_Event &a_event)
{
	SDLKey key = a_event.key.keysym.sym;

	if( a_event.type == SDL_KEYDOWN )
	{
		if(m_keys[key] == KEY_NONE || m_keys[key] == KEY_RELEASED)
			m_keys[key] = KEY_PRESSED;

		if(m_keys[key] == KEY_PRESSED)
			m_keys[key] = KEY_HELD;

	}
	else if( a_event.type == SDL_KEYUP && m_keys[key] == KEY_NONE)
		m_keys[key] = KEY_NONE;

	else if( a_event.type == SDL_KEYUP && m_keys[key] == KEY_HELD)
		m_keys[key] = KEY_RELEASED;

	else if( a_event.type == SDL_KEYUP && m_keys[key] == KEY_RELEASED)
		m_keys[key] = KEY_NONE;

}


void Keyboard::flushAllKeys()
{
	memset(m_keys, KEY_NONE, sizeof(KeyStates)*SDLK_LAST);
}

KeyStates Keyboard::GetKeyStates(SDLKey a_key)
{
	char c = a_key;

	if (a_key < 0 || a_key >=SDLK_LAST)
		return KEY_NONE;

	return m_keys[a_key];
}

int Keyboard::DetectKeysReleased()
{
	char c;
	//
	for (int i=33; i<123; i++)
	{
		if (m_keys[i] == KEY_RELEASED)
		{
			//Reset to NONE after releasing key
			m_keys[i] = KEY_NONE;
			return i;
		}
	}

	if (m_keys[SDLK_SPACE] == KEY_RELEASED)
	{
		//Reset to NONE after releasing key
		m_keys[SDLK_SPACE] = KEY_NONE;
		return SDLK_SPACE;
	}

	return 0;
}

int Keyboard::DetectOneKeyReleased(SDLKey a_key)
{
	if (m_keys[a_key] == KEY_RELEASED)
	{
		//Reset to NONE after releasing key
		m_keys[a_key] = KEY_NONE;
		return a_key;
	}

	return 0;
}