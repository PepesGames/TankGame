#include "TextFont.h"

#include "sdl_openGL.h"
#include "sdl.h"
#include "sdl_image.h"

TextFont::TextFont(void)
{
}

TextFont::TextFont(const char* szFilename)
{
	m_textfont = new Sprite(szFilename);
}

TextFont::~TextFont(void)
{
}


void TextFont::InsertProfile(char a_c)
{
	m_szProfile += a_c;
}

void TextFont::BackspaceProfile()
{
	if (m_szProfile.size() != 0)
	{
		int strlen = m_szProfile.size();
		m_szProfile.erase(strlen-1,1);
	}
}

void TextFont::ResetProfile()
{
	m_szProfile = "";
}

void TextFont::Draw()
{ 

	m_textfont->DrawString("2D Game Tanks", 300, 5, 40, 60, 1);

	//Print user profile
	const char * tmpProfile = m_szProfile.c_str();
	m_textfont->DrawString(tmpProfile, 200, 300, 20, 30, 1);

}

void TextFont::DrawTxt(string a_str, float a_x, float a_y, float a_w, float a_h, float a_scale)
{
	m_textfont->DrawString(a_str.c_str(), a_x, a_y, a_w, a_h, a_scale);
}

void TextFont::DrawScore(int a_p1Score, int a_p2Score)
{
	char tmpS[30];

	sprintf(tmpS, "Tank 1: %i", a_p1Score);
	m_textfont->DrawString(tmpS, 75, 70, 20, 30, 1);

	sprintf(tmpS, "Tank 2: %i", a_p2Score);
	m_textfont->DrawString(tmpS, 780, 70, 20, 30, 1);
}
