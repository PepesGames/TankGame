// XML.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "tinyxml.h"
#include <iostream>
#include <list>

using namespace std;

struct HighScore{
	string playerName;
	int score;
};

bool compare_scores(HighScore score1, HighScore score2)
{
	return score1.score > score2.score;
	/*int i = strcmp(score1.playerName.c_str(), score2.playerName.c_str());
	if(i<0)
		return true;
	else 
		return false;*/
}

//bool compare_intScores(int score1, int score2)
//{
//	return score1 > score2;
//}

void LoadXMLHScore(list<HighScore>& a_highScoresList, const char* a_filename);
void SaveXMLHScore(list<HighScore>& a_highScoresList, const char* a_filename);

int _tmain(int argc, _TCHAR* argv[])
{
	//list<int> myList;
	list<HighScore> g_highScoresList;
	HighScore garyScore = {"Gary", 1500};
	g_highScoresList.push_back(garyScore);


	//Loading XML doucment
	LoadXMLHScore(g_highScoresList, "scores.xml");

	//Sort the elements in the container from lower to higher
	g_highScoresList.sort(compare_scores);
	//myList.sort(compare_intScores);

	//Remove last element in the list container
	g_highScoresList.pop_back();

	//Saving XML doucment
	SaveXMLHScore(g_highScoresList, "scores2.xml");

	system("pause");
	return 0;
}


void LoadXMLHScore(list<HighScore>& a_highScoresList, const char* a_filename)
{
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
			cout << szScore << endl;

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


void SaveXMLHScore(list<HighScore>& a_highScoresList, const char* a_filename)
{
	list<HighScore>::iterator it;
	it = a_highScoresList.begin();

	//Saving XML doucment	
	TiXmlDocument doc2(a_filename);	
	TiXmlElement newHighScores("highscores");
	
	char val[10];

	for (it=a_highScoresList.begin(); it!=a_highScoresList.end(); it++)
	{
		//	cout << " " << (*it).playerName.c_str() << " " << (*it).score << std::endl;

		TiXmlElement score("score");

		score.SetAttribute("name", (*it).playerName.c_str());
		sprintf(val, "%i", (*it).score);
		TiXmlText txt(val);

		score.InsertEndChild(txt);

		newHighScores.InsertEndChild(score);
	}
	
	doc2.InsertEndChild(newHighScores);
	doc2.SaveFile();
}
