//#ifndef TEXTFONT_H
//#define TEXTFONT_H
#pragma once

//#include "Node.h"
#include <string>
#include "Sprite.h"

using namespace std;

class TextFont
{
public:
	TextFont(void);
	TextFont(const char* szFilename);
	~TextFont(void);

	//void Create(string a_text);

	void Draw();

	void DrawTxt(string a_str, float a_x, float a_y, float a_w, float a_h, float a_scale);
	void DrawHUD();
	void DrawScore(int a_p1score, int a_p2Score);
	
	void InsertProfile(char a_c);
	void BackspaceProfile();
	void ResetProfile();

	void Update(float dt) {}
	//Vector3 GetPosition();

	float GetRadius() { return 0; }

	//bool TestCollide(Node* a_node2) {return false;}

	void Destroy() {}
	float GetWidth() {return 0;}
	float GetHeight() {return 0;}

	string m_szProfile;
	Sprite* m_textfont;

private:

	Vector3 size;
	unsigned int m_handle;

};

