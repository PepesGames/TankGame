#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "stdafx.h"
#include "tinyxml.h"
#include "Matrix3x3.h"
#include "Keyboard.h"
#include "Missile.h"
#include <vector>
#include <list>
#include <string>

using namespace std;

enum State {
	STARTUP,
	SPLASH, 
	MAIN_MENU,
	GAME,
	HIGHSCORE,
	SELECT_PROFILE,
	CREATE_PROFILE1,
	CREATE_PROFILE2,
	EXIT = -1
};

enum SubState {
	SELECT_PROFILE1,
	SELECT_PROFILE2,
	CREATE_P1,
	CREATE_P2,
	SELECTED_PROFILES
};

struct HighScore {
	string playerName;
	int score;
};

struct Profile {
	unsigned int id;
	char Name[20];
	unsigned int num_played;
	unsigned int num_won;
	unsigned int max_score;
};

// predefine classes
class Tank;
class Wall;
class Keyboard;
class TextFont;

class GameController
{
public:

	GameController(int a_windowWidth, int a_windowHeight, bool a_fullscreen = false);
	~GameController();

	// function returns when the game loop exits
	void Run();
	static void createMissile(Matrix3x3& a_matrix, Vector3 a_velocity, FACTION a_eFaction);
	static void createWall(float a_x, float a_y, const char* a_filename);

	// Load XML file for High Scores
	void LoadXMLHScore(list<HighScore>& a_highScoresList, const char* a_filename);


protected:

	void InitSDLOpenGL();
	void CloseSDLOpenGL();

	void LoadAssets();
	void LoadLevelMap();

	void FreeAssets();

	void ProcessEvents();
	void Update(float a_dT);
	void Draw();


	int m_windowWidth;
	int m_windowHeight;
	bool m_fullscreen;
	bool m_gameOver;


private:

	int m_framecount;
	Keyboard* m_keyboard;

	Tank* m_player1;
	Tank* m_player2;
	TextFont* m_textfont;
	Sprite* m_logo;
	Sprite* m_background;
	//JMMKK
	Sprite* m_splashscreen;
	char* m_Wallfile;
	char* m_XMLfile;
	char* m_BINfile;

	static const int LVLPATH = 1; 
	static const int LVLWALL = 2;

	static class GridIndex* m_pkGridIndex;

	int m_keySelect;
	int m_profileSelect;
	State g_GameState;
	State g_NextGameState;

	SubState g_SState;
	SubState g_NextSState;

	// Linked list of High Score
	list<HighScore> g_highScoresList;

	// Linked list of Players' Profile
	list<Profile> g_ProfilesList;
};

#endif