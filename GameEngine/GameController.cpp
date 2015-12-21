#include "GameController.h"
#include "sdl_openGL.h"
#include "sdl.h"

#include "Tank.h"
#include "Wall.h"
#include "Keyboard.h"
#include "TextFont.h"

#include "GridIndex.h"

int highScore = 0;

class Tank;



GridIndex* GameController::m_pkGridIndex = nullptr;


bool compare_scores(HighScore score1, HighScore score2)
{ 
	return score1.score > score2.score; 
}

bool compare_profiles(Profile profile1, Profile profile2)
{
	return (profile1.Name == profile2.Name);
}

GameController::GameController(int a_windowWidth, int a_windowHeight, bool a_fullscreen)
{

	m_windowWidth	= a_windowWidth;
	m_windowHeight	= a_windowHeight;
	m_fullscreen	= a_fullscreen;
	m_gameOver		= false;

	m_framecount = 0;
	m_keyboard = new Keyboard();


	g_GameState = STARTUP;
	//g_NextGameState = MAIN_MENU;
	g_NextGameState = SPLASH;
	m_keySelect = 0;
	m_profileSelect = 0;

}


GameController::~GameController()
{
	delete m_keyboard;
}


// function returns when the game loop exits
void GameController::Run()
{
	InitSDLOpenGL();

	LoadAssets();

	Uint32 startTime = SDL_GetTicks();
	float deltaTime;
	Uint32 time;

	//while( !m_gameOver )
	while( !m_gameOver && g_GameState != EXIT)
	{
		//Calculate Delta time
		time = SDL_GetTicks();
		deltaTime = (time - startTime) * 0.001f;

		glClear(GL_COLOR_BUFFER_BIT);

		//Proceed to Next Game State
		g_GameState = g_NextGameState;

		//Proceed to Next Game SubState
		g_SState = g_NextSState;

		ProcessEvents();

		Update(deltaTime);
		Draw();

		SDL_GL_SwapBuffers();

		startTime = time;
	}

	FreeAssets();

	CloseSDLOpenGL();
}


void GameController::InitSDLOpenGL()
{
	// Setup SDL Window parameters
	//------------------------------
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK);

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER,	1	);
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE,		32	);
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE,		8	);
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE,		8	);
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE,		8	);
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE,		8	);

	unsigned int flags = SDL_OPENGL | SDL_HWSURFACE;

	if( m_fullscreen )
		flags = flags | SDL_FULLSCREEN;

	SDL_SetVideoMode( m_windowWidth, m_windowHeight, 32, flags );
	//------------------------------


	// Setup OpenGL
	//------------------------------

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f);

	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f);


	// setup orthgraphic projection for 2D
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( 0.0f, m_windowWidth, m_windowHeight, 0.0f, 0.0f, 10.0f);

	glEnable(GL_TEXTURE_2D);

	// enable blending function for transparency
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	// Enable / Disable V-Sync
	// This is Not Cross Platform
	typedef void (APIENTRY * WGLSWAPINTERVALEXT)(int);
	WGLSWAPINTERVALEXT wglSwapIntervalEXT = (WGLSWAPINTERVALEXT)wglGetProcAddress("wglSwapIntervalEXT");

	if( wglSwapIntervalEXT )
	{
		wglSwapIntervalEXT(0);
	}


}


void GameController::CloseSDLOpenGL()
{
	// do other cleanup operations here
	SDL_Quit();
}


void GameController::ProcessEvents()
{
	SDL_Event e;
	while( SDL_PollEvent( &e ) )
	{
		if( e.type == SDL_QUIT )
			m_gameOver = true;

		if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
			m_keyboard->ProcessEvent(e);

	}
}


void GameController::LoadAssets()
{

	m_pkGridIndex = new GridIndex(12, 8);

	//Loading Wall file
	m_Wallfile = "./Images/wall.png";

	this->LoadLevelMap();

	m_splashscreen = new Sprite("./Images/PepeLogo.png");

	m_logo = new Sprite("./Images/title.png");

	m_background = new Sprite("./Images/Fondo1.png");

	m_player1 = new Tank("./Images/tank1_body.png", "./Images/tank1_turret.png", 900, 400, FT_TANK_GREEN);
	m_player2 = new Tank("./Images/tank2_body.png","./Images/tank2_turret.png", 125, 400, FT_TANK_RED);

	m_textfont = new TextFont("./Images/font7.png");


	//Loading HighScore from XML file
	m_XMLfile = "./Data/highscores.xml";
	LoadXMLHScore(g_highScoresList, m_XMLfile);

	m_pkGridIndex->AddNode(m_player1);
	m_pkGridIndex->AddNode(m_player2);

}


void GameController::FreeAssets()
{
	delete m_pkGridIndex;

	if( m_player1 != NULL )
	{
		//TODO free player tank and turret sprites
		delete m_player1;
	}

	if( m_player2 != NULL )
	{
		//TODO free player tank and turret sprites
		delete m_player2;
	}

}


void GameController::Update(float dT)
{	
	if (g_GameState == MAIN_MENU)
	{
		//If RETURN KEY PRESSED
		int inKey = m_keyboard->DetectOneKeyReleased(SDLK_RETURN);
		if (inKey != 0)
		{
			switch (m_keySelect) {
			case 0:
				g_NextGameState = GAME;
				break;
			case 1:
				g_NextGameState = HIGHSCORE;
				break;
			case 2:
				g_NextGameState = SELECT_PROFILE;
				g_NextSState = SELECT_PROFILE1;
				break;
			case 3:
				g_NextGameState = EXIT;
				break;
			default:
				break;
			}
		}

		//If '1' KEY PRESSED
		inKey = m_keyboard->DetectOneKeyReleased(SDLK_1);
		if (inKey != 0)
			g_NextGameState = GAME;

		//If '2' KEY PRESSED
		inKey = m_keyboard->DetectOneKeyReleased(SDLK_2);
		if (inKey != 0)
			g_NextGameState = HIGHSCORE;

		//If '3' KEY PRESSED
		inKey = m_keyboard->DetectOneKeyReleased(SDLK_3);
		if (inKey != 0)
			g_NextGameState = SELECT_PROFILE;

		//If 'ARROW UP' KEY PRESSED
		inKey = m_keyboard->DetectOneKeyReleased(SDLK_UP);
		if (inKey != 0)
			if (m_keySelect > 0)
				m_keySelect -= 1;

		//If 'ARROW DOWN' KEY PRESSED
		inKey = m_keyboard->DetectOneKeyReleased(SDLK_DOWN);
		if (inKey != 0)
			if (m_keySelect < 3)
				m_keySelect += 1;

	}
	else if (g_GameState == GAME)
	{
		m_pkGridIndex->Update(dT);

		/***********************************************************/
		//	Player 1 Key Control
		/***********************************************************/

		//If RIGHT KEY PRESSED
		if(m_keyboard->GetKeyStates(SDLK_RIGHT) == KEY_HELD)
			m_player1->Rotate(-50.0f * dT);

		//If LEFT KEY PRESSED
		if(m_keyboard->GetKeyStates(SDLK_LEFT) == KEY_HELD)
			m_player1->Rotate(50.0f * dT);

		//If UP KEY PRESSED
		if(m_keyboard->GetKeyStates(SDLK_UP) == KEY_HELD)
		{
			//moving forward
			m_player1->m_acc.m_X += 0.0f;
			m_player1->m_acc.m_Y -= 2.0f;

			if(m_player1->m_acc.m_Y < -100.0f)
				m_player1->m_acc.m_Y = -100.0f;

			m_player1->Translate(0.0f, m_player1->m_acc.m_Y * dT);
		}
		else if (m_player1->m_acc.m_Y  < 0)
		{
			m_player1->m_acc.m_Y += 2.0f;
			if(m_player1->m_acc.m_Y > 0)
				m_player1->m_acc.m_Y = 0.0f;

			m_player1->Translate(0.0f, m_player1->m_acc.m_Y * dT);	
		}

		//If DOWN KEY PRESSED
		if(m_keyboard->GetKeyStates(SDLK_DOWN) == KEY_HELD)
		{
			//moving backward
			m_player1->m_acc.m_X += 0.0f;
			m_player1->m_acc.m_Y += 2.0f;

			if(m_player1->m_acc.m_Y > 100.0f)
				m_player1->m_acc.m_Y = 100.0f;

			m_player1->Translate(0.0f, m_player1->m_acc.m_Y * dT);	
		}
		else if (m_player1->m_acc.m_Y  > 0)
		{
			m_player1->m_acc.m_Y -= 2.0f;
			if(m_player1->m_acc.m_Y < 0)
				m_player1->m_acc.m_Y = 0.0f;

			m_player1->Translate(0.0f, m_player1->m_acc.m_Y * dT);	
		}

		//If RIGHT SHIFT KEY PRESSED
		if(m_keyboard->GetKeyStates(SDLK_RSHIFT) == KEY_HELD)
		{
			m_player1->RotateTurret(50.0f * dT);
		}

		//If RIGHT CTRL KEY PRESSED
		if(m_keyboard->GetKeyStates(SDLK_RCTRL) == KEY_HELD)
		{
			m_player1->RotateTurret(-50.0f * dT);
		}

		//If RETURN KEY PRESSED
		if(m_keyboard->GetKeyStates(SDLK_RETURN) == KEY_HELD)
		{
			m_player1->Shoot();		//Fire Missile
		}

		/***********************************************************/
		//	Player 2 Key Control
		/***********************************************************/

		//If RIGHT KEY PRESSED
		if(m_keyboard->GetKeyStates(SDLK_d) == KEY_HELD)
		{
			m_player2->Rotate(-50.0f * dT);
		}

		//If LEFT KEY PRESSED
		if(m_keyboard->GetKeyStates(SDLK_a) == KEY_HELD)
		{
			m_player2->Rotate(50.0f * dT);
		}

		//If UP KEY PRESSED
		if(m_keyboard->GetKeyStates(SDLK_w) == KEY_HELD)
		{
			m_player2->m_acc.m_X += 0.0f;
			m_player2->m_acc.m_Y -= 2.0f;

			if(m_player2->m_acc.m_Y < -100.0f)
				m_player2->m_acc.m_Y = -100.0f;

			m_player2->Translate(0.0f, m_player2->m_acc.m_Y * dT);
		}
		else if (m_player2->m_acc.m_Y  < 0)
		{
			m_player2->m_acc.m_Y += 2.0f;
			if(m_player2->m_acc.m_Y > 0)
				m_player2->m_acc.m_Y = 0.0f;

			m_player2->Translate(0.0f, m_player2->m_acc.m_Y * dT);	
		}

		//If DOWN KEY PRESSED
		if(m_keyboard->GetKeyStates(SDLK_s) == KEY_HELD)
		{
			m_player2->m_acc.m_X += 0.0f;
			m_player2->m_acc.m_Y += 2.0f;

			if(m_player2->m_acc.m_Y > 100.0f)
				m_player2->m_acc.m_Y = 100.0f;

			m_player2->Translate(0.0f, m_player2->m_acc.m_Y * dT);
		}
		else if (m_player2->m_acc.m_Y  > 0)
		{
			m_player2->m_acc.m_Y -= 2.0f;
			if(m_player2->m_acc.m_Y < 0)
				m_player2->m_acc.m_Y = 0.0f;

			m_player2->Translate(0.0f, m_player2->m_acc.m_Y * dT);	
		}

		//If LEFT SHIFT KEY PRESSED
		if(m_keyboard->GetKeyStates(SDLK_LSHIFT) == KEY_HELD)
			m_player2->RotateTurret(50.0f * dT);

		//If LEFT CTRL KEY PRESSED
		if(m_keyboard->GetKeyStates(SDLK_LCTRL) == KEY_HELD)
			m_player2->RotateTurret(-50.0f * dT);

		//If 'Q' KEY PRESSED
		if(m_keyboard->GetKeyStates(SDLK_q) == KEY_HELD)
			m_player2->Shoot();		//Fire Missile

		//If ESCAPE KEY PRESSED
		int inKey = m_keyboard->DetectOneKeyReleased(SDLK_ESCAPE);
		if (inKey != 0)
			g_NextGameState = MAIN_MENU;

	}	// end if else statement
	else if (g_GameState == HIGHSCORE)
	{
		//LoadXMLHScore(g_highScoresList, m_XMLfile);
		g_highScoresList.sort(compare_scores);

		//If ESCAPE KEY PRESSED
		int inKey = m_keyboard->DetectOneKeyReleased(SDLK_ESCAPE);
		if (inKey != 0)
			g_NextGameState = MAIN_MENU;

	}
	else if (g_GameState == SELECT_PROFILE)
	{
		//Create unique list
		//g_ProfilesList.unique(compare_profiles);

		if (g_SState == CREATE_P1)
		{
			//Get User Profile1 Input
			int inKey = m_keyboard->DetectKeysReleased();
			if (inKey != 0)
				m_textfont->InsertProfile(inKey);

			//If 'BACKSPACE' KEY PRESSED
			inKey = m_keyboard->DetectOneKeyReleased(SDLK_BACKSPACE);
			if (inKey != 0)
				m_textfont->BackspaceProfile();

			//If 'RETURN' KEY PRESSED
			inKey = m_keyboard->DetectOneKeyReleased(SDLK_RETURN);
			if (inKey != 0)
			{
				m_player1->m_szProfileName = m_textfont->m_szProfile;

				//Go to PROFILE2 SState
				g_NextSState = CREATE_P2;

				//Reset tmp array storing Profile1
				m_textfont->m_szProfile = "";
			}
		}


		//If RETURN KEY PRESSED
		int inKey = m_keyboard->DetectOneKeyReleased(SDLK_RETURN);
		if (inKey != 0)
		{
			if (m_profileSelect == g_ProfilesList.size())
			{
				g_NextSState = CREATE_P1;
			}
			else if (g_SState == SELECT_PROFILE1)
			{
				list<Profile>::iterator it;
				for (it=g_ProfilesList.begin(); it!=g_ProfilesList.end(); it++)
				{
					if ((*it).id == m_profileSelect)
						m_player1->m_szProfileName = (*it).Name;
				}
				g_NextSState = SELECT_PROFILE2;
			}
			else if (g_SState == SELECT_PROFILE2)
			{
				list<Profile>::iterator it;
				for (it=g_ProfilesList.begin(); it!=g_ProfilesList.end(); it++)
				{
					if ((*it).id == m_profileSelect)
						m_player2->m_szProfileName = (*it).Name;
				}
				g_NextSState = SELECTED_PROFILES;
			}
			else if (g_SState == SELECTED_PROFILES)
			{
				g_NextGameState = MAIN_MENU;
			}
			else if (g_SState == CREATE_P1)
			{
				//Get User Profile1 Input
				int inKey = m_keyboard->DetectKeysReleased();
				if (inKey != 0)
					m_textfont->InsertProfile(inKey);

				//If 'BACKSPACE' KEY PRESSED
				inKey = m_keyboard->DetectOneKeyReleased(SDLK_BACKSPACE);
				if (inKey != 0)
					m_textfont->BackspaceProfile();

				//If 'RETURN' KEY PRESSED
				inKey = m_keyboard->DetectOneKeyReleased(SDLK_RETURN);
				if (inKey != 0)
				{
					m_player1->m_szProfileName = m_textfont->m_szProfile;

					//Go to PROFILE2 SState
					g_NextSState = CREATE_P2;

					//Reset tmp array storing Profile1
					m_textfont->m_szProfile = "";
				}

				//If ESCAPE KEY PRESSED
				inKey = m_keyboard->DetectOneKeyReleased(SDLK_ESCAPE);
				if (inKey != 0)
					g_NextSState = SELECT_PROFILE1;

			}


		}

		//If 'ARROW UP' KEY PRESSED
		inKey = m_keyboard->DetectOneKeyReleased(SDLK_UP);
		if (inKey != 0)
			if (m_profileSelect > 0)
				m_profileSelect -= 1;

		//If 'ARROW DOWN' KEY PRESSED
		inKey = m_keyboard->DetectOneKeyReleased(SDLK_DOWN);
		if (inKey != 0)
			if (m_profileSelect < g_ProfilesList.size())
				m_profileSelect += 1;


		//If ESCAPE KEY PRESSED
		inKey = m_keyboard->DetectOneKeyReleased(SDLK_ESCAPE);
		if (inKey != 0)
			g_NextGameState = MAIN_MENU;

	}
	else if (g_GameState == CREATE_PROFILE1)
	{
		//Get User Profile1 Input
		int inKey = m_keyboard->DetectKeysReleased();
		if (inKey != 0)
			m_textfont->InsertProfile(inKey);

		//If 'BACKSPACE' KEY PRESSED
		inKey = m_keyboard->DetectOneKeyReleased(SDLK_BACKSPACE);
		if (inKey != 0)
			m_textfont->BackspaceProfile();

		//If 'RETURN' KEY PRESSED
		inKey = m_keyboard->DetectOneKeyReleased(SDLK_RETURN);
		if (inKey != 0)
		{
			m_player1->m_szProfileName = m_textfont->m_szProfile;

			//Go to PROFILE2 state
			g_NextGameState = CREATE_PROFILE2;

			//Reset tmp array storing Profile1
			m_textfont->m_szProfile = "";
		}

		//If ESCAPE KEY PRESSED
		inKey = m_keyboard->DetectOneKeyReleased(SDLK_ESCAPE);
		if (inKey != 0)
			g_NextGameState = MAIN_MENU;

	}

	else if (g_GameState == SPLASH)
	{


		//If ESCAPE KEY PRESSED
		int inKey = m_keyboard->DetectOneKeyReleased(SDLK_ESCAPE);
		if (inKey != 0)
			g_NextGameState = MAIN_MENU;
	}

	else if (g_GameState == CREATE_PROFILE2)
	{
		//Get User Profile2 Input
		int inKey = m_keyboard->DetectKeysReleased();
		if (inKey != 0)
			m_textfont->InsertProfile(inKey);

		//If 'BACKSPACE' KEY PRESSED
		inKey = m_keyboard->DetectOneKeyReleased(SDLK_BACKSPACE);
		if (inKey != 0)
			m_textfont->BackspaceProfile();

		//If 'RETURN' KEY PRESSED
		inKey = m_keyboard->DetectOneKeyReleased(SDLK_RETURN);
		if (inKey != 0)
		{
			m_player2->m_szProfileName = m_textfont->m_szProfile;

			//Return to MAIN_MENU state
			g_NextGameState = MAIN_MENU;
		}

		//If ESCAPE KEY PRESSED
		inKey = m_keyboard->DetectOneKeyReleased(SDLK_ESCAPE);
		if (inKey != 0)
			g_NextGameState = MAIN_MENU;

	}	//end else if

	m_framecount++;
	Sleep(30);
}


void GameController::Draw()
{
	//Draw Background Tile
	for (int i=0; i<16; i++)
		for (int j=0; j<12; j++)
			m_background->DrawImage(64*i, 64*j, 64, 64);

	if (g_GameState == MAIN_MENU) 
	{
		m_logo->DrawImage(220, 0, 600, 850);
		m_textfont->DrawTxt(">", 300, 500 + m_keySelect * 50, 20, 30, 1);
		m_textfont->DrawTxt("1 - Begin Game", 350, 500, 20, 30, 1);
		m_textfont->DrawTxt("2 - High Scores", 350, 550, 20, 30, 1);
		m_textfont->DrawTxt("3 - Select/Create Profile", 350, 600, 20, 30, 1);
		m_textfont->DrawTxt("4 - Quit", 350, 650, 20, 30, 1);
		m_textfont->DrawTxt("Press 'Enter' to Select", 400, 700, 10, 15, 1);
	}

	//Draw splash screen
	else if (g_GameState == SPLASH) 
	{
		m_splashscreen->DrawImage(280, 150, 500, 400);
		m_textfont->DrawTxt("Press 'Esc'", 400, 700, 20, 30, 1);
	}

	else if (g_GameState == GAME)
	{

		m_pkGridIndex->DrawGrid();
		//m_textfont->DrawTxt("Game's Tanks", 300, 5, 40, 60, 1);

		char tmpNum[6];
		string tmpScore;

		if (m_player1->m_szProfileName == "")
		{
			tmpScore = "Tank 1: ";
			tmpScore = tmpScore.append(itoa(m_player1->m_score, tmpNum, 10));
		}
		else
			tmpScore = m_player1->m_szProfileName + ": " + itoa(m_player1->m_score, tmpNum, 10);


		//Draw Player1 Score
		m_textfont->DrawTxt(tmpScore, 75, 70, 20, 30, 1);

		if (m_player2->m_szProfileName == "")
		{
			tmpScore = "Tank 2: ";
			tmpScore = tmpScore.append(itoa(m_player2->m_score, tmpNum, 10));
		}
		else
			tmpScore = m_player2->m_szProfileName + ": " + itoa(m_player2->m_score, tmpNum, 10);

		//Draw Player2 Score
		m_textfont->DrawTxt(tmpScore, 780, 70, 20, 30, 1);

	}
	else if (g_GameState == HIGHSCORE)
	{
		m_textfont->DrawTxt("High Scores", 300, 5, 40, 60, 1);

		list<HighScore>::iterator it;

		int iloc = 2;
		for (it=g_highScoresList.begin(); it!=g_highScoresList.end(); it++)
		{
			char tmpBuff[5];
			itoa(iloc-1, tmpBuff, 10);
			m_textfont->DrawTxt(tmpBuff, 250, 60 * iloc, 20, 30, 1);
			m_textfont->DrawTxt(".", 270, 60 * iloc, 20, 30, 1);
			m_textfont->DrawTxt((*it).playerName.c_str(), 300, 60 * iloc, 20, 30, 1);

			char tmpscore[10];			
			itoa((*it).score, tmpscore, 10);
			m_textfont->DrawTxt(tmpscore, 500, 60 * iloc, 20, 30, 1);
			iloc++;
		}
	}
	else if (g_GameState == SELECT_PROFILE)
	{
		m_textfont->DrawTxt("Select Profile", 250, 5, 40, 60, 1);
		m_textfont->DrawTxt("Played", 400, 120, 20, 30, 1);
		m_textfont->DrawTxt("Won", 625, 120, 20, 30, 1);
		m_textfont->DrawTxt("Max Score", 800, 120, 20, 30, 1);

		if (g_SState == SELECT_PROFILE1)
			m_textfont->DrawTxt("Player 1 >", 70, 50 + 60 * (m_profileSelect + 2), 20, 30, 1);
		else
			m_textfont->DrawTxt("Player 2 >", 70, 50 + 60 * (m_profileSelect + 2), 20, 30, 1);

		list<Profile>::iterator it;

		int iloc = 2;
		for (it=g_ProfilesList.begin(); it!=g_ProfilesList.end(); it++)
		{
			m_textfont->DrawTxt((*it).Name, 150, 50 + 60 * iloc, 20, 30, 1);

			char tmpscore[10];			
			itoa((*it).num_played, tmpscore, 10);
			m_textfont->DrawTxt(tmpscore, 450, 50 + 60 * iloc, 20, 30, 1);

			itoa((*it).num_won, tmpscore, 10);
			m_textfont->DrawTxt(tmpscore, 650, 50 + 60 * iloc, 20, 30, 1);

			itoa((*it).max_score, tmpscore, 10);
			m_textfont->DrawTxt(tmpscore, 850, 50 + 60 * iloc, 20, 30, 1);

			iloc++;
		}

		m_textfont->DrawTxt("Create New Profile", 150, 50 + 60 * iloc, 20, 30, 1);
		iloc++;

		if (g_SState == SELECT_PROFILE1)
			m_textfont->DrawTxt("Press 'Enter' to Select Player 1", 400, 50 + 60 * iloc, 10, 15, 1);
		else if (g_SState == SELECT_PROFILE2)
			m_textfont->DrawTxt("Press 'Enter' to Select Player 2", 400, 50 + 60 * iloc, 10, 15, 1);
		else
			m_textfont->DrawTxt("Press 'Enter' to Confirm", 400, 50 + 60 * iloc, 10, 15, 1);

		iloc++;

		if (g_SState == SELECT_PROFILE2 || g_SState == SELECTED_PROFILES || g_SState == CREATE_P1)
		{
			m_textfont->DrawTxt("Player 1:", 350, 50 + 60 * iloc, 20, 30, 1);

			//Print user profile
			m_textfont->DrawTxt(m_player1->m_szProfileName, 550, 50 + 60 * iloc, 20, 30, 1);
		}

		iloc++;

		if (g_SState == SELECTED_PROFILES)
		{
			m_textfont->DrawTxt("Player 2:", 350, 50 + 60 * iloc, 20, 30, 1);

			//Print user profile
			m_textfont->DrawTxt(m_player2->m_szProfileName, 550, 50 + 60 * iloc, 20, 30, 1);
		}

	}
	else if (g_GameState == CREATE_PROFILE1)
	{
		m_textfont->DrawTxt("Enter Player 1 Name:", 300, 250, 20, 30, 1);
		m_textfont->DrawTxt(">>", 300, 300, 20, 30, 1);
		//Print user profile
		string tmpProfile = m_textfont->m_szProfile;
		m_textfont->DrawTxt(tmpProfile, 350, 300, 20, 30, 1);
	}
	else if (g_GameState == CREATE_PROFILE2)
	{

		m_textfont->DrawTxt("Enter Player 1 Name:", 300, 250, 20, 30, 1);
		m_textfont->DrawTxt(">>", 300, 300, 20, 30, 1);
		//Print user profile
		m_textfont->DrawTxt(m_player1->m_szProfileName, 350, 300, 20, 30, 1);

		m_textfont->DrawTxt("Enter Player 2 Name:", 300, 400, 20, 30, 1);
		m_textfont->DrawTxt(">>", 300, 450, 20, 30, 1);
		//Print user profile
		string tmpProfile = m_textfont->m_szProfile;
		m_textfont->DrawTxt(tmpProfile, 350, 450, 20, 30, 1);

	}

}


void GameController::createMissile(Matrix3x3& a_matrix, Vector3 a_velocity, FACTION a_eFaction)
{
	Missile* pMissile = new Missile(a_matrix, "./Images/missile.png", a_velocity, a_eFaction);

	if (m_pkGridIndex != nullptr)
		m_pkGridIndex->AddNode(pMissile);

}


void GameController::createWall(float a_x, float a_y, const char* a_filename)
{

	Wall* pWall = new Wall(a_filename, a_x, a_y);

	pWall->SetFaction(FT_WALL);

	if (m_pkGridIndex != nullptr)
		m_pkGridIndex->AddNode(pWall);

}




////////////////////////////////////////////////////////////
// @brief Loads a bitmap and uses colors to represent tile types
////////////////////////////////////////////////////////////
void GameController::LoadLevelMap()
{
	BITMAPFILEHEADER oBMPHeader;
	BITMAPINFOHEADER oBMPInfo;

	// read binary file
	//JMMKK
	FILE* pFile = fopen("./Images/level003.bmp","rb");

	// read both headers
	fread(&oBMPHeader,sizeof(BITMAPFILEHEADER),1,pFile);
	fread(&oBMPInfo,sizeof(BITMAPINFOHEADER),1,pFile);

	unsigned int uiWidth = oBMPInfo.biWidth;
	unsigned int uiHeight = oBMPInfo.biHeight;

	struct Pixel24
	{
		char r, g, b;
	};

	Pixel24* aoPixels = new Pixel24[ oBMPInfo.biWidth * oBMPInfo.biHeight ];

	// seek to the location in the file where the pixels start
	fseek(pFile,oBMPHeader.bfOffBits,SEEK_SET);
	fread(aoPixels,sizeof(Pixel24),oBMPInfo.biWidth * oBMPInfo.biHeight,pFile); 

	Pixel24* curPos = aoPixels;
	for (int y = 0; y < oBMPInfo.biHeight; y++)
	{
		for (int x = 0; x < oBMPInfo.biWidth; x++)
		{
			//get pixel and save it to array using switch
			Pixel24 pixel = *curPos;
			curPos++;

			int r = (pixel.r&0xff)<<16;
			int g = (pixel.g&0xff)<<8;
			int b = (pixel.b&0xff);

			int p = r|b|g;

			switch(p)
			{
			case 0x00000000:
				this->createWall(x*64+32, y*64+32, m_Wallfile);
				break;
			default:
				break; 
			}
		}
	}

	fclose(pFile);
	delete [] aoPixels;
}


void GameController::LoadXMLHScore(list<HighScore>& a_highScoresList, const char* a_filename)
{
	//Clear High Scores List
	a_highScoresList.clear();

	//Loading XML doucment
	TiXmlDocument doc(a_filename);
	bool bLoadedOkay = doc.LoadFile();
	HighScore tmpScore;

	if (bLoadedOkay)
	{
		TiXmlElement* pHScoresElement = doc.FirstChildElement(); 
		TiXmlElement* pScoreElement = pHScoresElement->FirstChildElement();
		while( pScoreElement )
		{	
			const char* szName = pScoreElement->Attribute("name");
			const char* szScore = pScoreElement->GetText();

			tmpScore.playerName = szName;
			tmpScore.score = atoi(szScore);

			a_highScoresList.push_back(tmpScore);

			pScoreElement = pScoreElement->NextSiblingElement();
		}


	}
	else
	{
		// Display Error
		cout << "error loading file" << endl;
	}
}

